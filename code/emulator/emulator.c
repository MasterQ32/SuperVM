#define _CRT_SECURE_NO_WARNINGS

#include "emulator.h"

#include "exp.h"
#include "devices/timer.h"
#include "disassembler.h"

#include <stdbool.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool running = true;
bool visualMode = false;

bool debugMode = false;
bool executionPaused = false;

device_t *devices[DEVICE_COUNT];
deviceconfig_t deviceConfigs[DEVICE_COUNT];

/**
* An assertion the VM does.
* @param assertion If zero, the assertion failed.
* @param msg       The message that should be shown when the assertion fails.
*/
void vm_assert(int assertion, const char *msg)
{
	if (assertion)
		return;
	fprintf(stderr, "Assertion failed: %s\n", msg);
	running = false;
}

/**
* The hosts syscall implementation.
* @param process The process that calls the syscall.
* @param info    Additional information for the syscall. Contains arguments and results.
*/
uint32_t vm_syscall(spu_t *process, cmdinput_t *info)
{
	switch (info->info)
	{
	case 0:
		running = false;
		return 0;
	case 1:
		fprintf(stdout, "%c", info->input0);
		fflush(stdout);
		return 0;
	case 2:
		fprintf(stdout, "%d", info->input0);
		fflush(stdout);
		return 0;
	default:
		fprintf(
			stderr,
			"SYSCALL [%d]: (%d, %d)\n",
			info->info,
			info->input0,
			info->input1);
		break;
	}
	return -1;
}

/**
* The hosts hardware IO implementation.
* @param process The process that wants to do IO.
* @param info    Additional information for the HWIO. Contains arguments and results.
*/
uint32_t vm_hwio(spu_t *process, cmdinput_t *info)
{
	// input1:  data
	// input0:  global register
	// cmdinfo: 0=read, 1=write

	// get IO device
	uint32_t deviceId = (info->input0 >> 16) & 0xFFFF;
	uint32_t regNum = (info->input0) & 0xFFFF;

	if (deviceId >= DEVICE_COUNT || devices[deviceId] == NULL)
	{
		// Invalid device
		fprintf(stderr, "Device %d not found.\n", deviceId);
		return -1;
	}

	device_t *device = devices[deviceId];
	if (info->info)
	{
		device->write(device, regNum, info->input1);
		return info->input1;
	}
	else
	{
		return device->read(device, regNum);
	}
}

spu_t mainCore;

void device_interrupt_callback(struct device *device, uint32_t interrupt)
{
	deviceconfig_t *config = device->tagPtr;
	mainCore.interrupt = config->interruptBase + interrupt;
}

void dump_vm()
{
	int width = 0;
	width += fprintf(stderr,
		"cp=%8X bp=%3d f=%1X [",
		mainCore.codePointer,
		mainCore.basePointer,
		mainCore.flags
		);

	for (uint32_t i = 0; i <= mainCore.stackPointer; i++)
	{
		width += fprintf(stderr, " %d", mainCore.stack[i]);
	}

	width += fprintf(stderr, " ] ");
	
	const char *padding = "                                                  ";
	if(width < strlen(padding))
		fprintf(stderr, "%s", &padding[width]);
	
	instruction_t *instr = (uint8_t*)mainCore.memory + (mainCore.codePointer << 3);
	disassemble(instr, 1, mainCore.codePointer, stderr);
}

void update_vm()
{
	int i;

	// first, step all the devices
	for (i = 0; i < DEVICE_COUNT; i++)
	{
		device_t *dev = devices[i];
		if (dev == NULL || dev->update == NULL)
			continue;
		dev->update(dev);
	}

	// then go for the main core
	vm_step_process(&mainCore);

	// Debug mode is for live debugging,
	// paused execution is for manual debugging 
	if (debugMode || executionPaused) dump_vm();
}

void initialize_vm()
{
	// Initialize memory
	mainCore.memoryBase = 0x00; // Linear memory, start at 0x00
	mainCore.memorySize = 0x4000000; // 64 MB;
	mainCore.memory = malloc(mainCore.memorySize);

	// Initialize code execution
	mainCore.code = mainCore.memory;
	mainCore.codeLength = mainCore.memorySize / sizeof(instruction_t);

	// Setup registers
	mainCore.codePointer = 0;
	mainCore.stackPointer = 0;
	mainCore.basePointer = 0;
	mainCore.flags = 0;

	mainCore.interrupt = 0;

	// Clear stack
	memset(mainCore.stack, 0, VM_STACKSIZE * sizeof(uint32_t));

	// Timer Device with IO Base 0x10000
	devices[1] = devtimer_create();
	deviceConfigs[1].interruptBase = 1;
	devices[1]->tagPtr = &deviceConfigs[1];
}

void load_section(const expsection_t * section, FILE *f)
{
	fprintf(stderr, "Loading %s to 0x%X\n", section->name, section->base);
	uint8_t *sectionInRam = (uint8_t*)mainCore.memory + section->base;
	int len = fread(sectionInRam, 1, section->length, f);
	if (len != section->length)
		fprintf(stderr, "Read invalid size.\n");
}


extern bool autoSwapBuffers;
extern int executionsPerSimulationStep;

int main(int argc, char **argv)
{
	// Required before ANY virtual machine memory operations...
	initialize_vm();

	opterr = 0;
	int c;
	while ((c = getopt(argc, argv, "dVs:R")) != -1)
	{
		switch (c)
		{
		case 'd':
			debugMode = 1;
			break;
		case 'V':
			visualMode = 1;
			break;
		case 'R':
			autoSwapBuffers = true;
			break;
		case 's':
			executionsPerSimulationStep = atoi(optarg);
			if (executionsPerSimulationStep <= 0)
				executionsPerSimulationStep = 50;
			break;
		case '?':
			if (optopt == 'o' || optopt == 'c' || optopt == 'd')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			return 1;
		default:
			abort();
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "No initial RAM-file.\n");
		exit(1);
	}

	for (int index = optind; index < argc; index++)
	{
		// fprintf(stderr, "Loading %s...\n", argv[index]);
		if (exp_load(argv[index], &load_section) == 0) {
			fprintf(stderr, "%s not found.\n", argv[index]);
			exit(1);
		}
	}

	if (debugMode) dump_vm();

	if (visualMode)
		run_visual_mode();
	else
		run_text_mode();

	return 0;
}
