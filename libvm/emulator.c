#define _CRT_SECURE_NO_WARNINGS

#include "vm.h"
#include "exp.h"

#include <stdbool.h>

#if defined(_MSC_VER)
#include <SDL.h>
#include "getopt.h"
#undef main
#else
#include <SDL/SDL.h>
#include <getopt.h>
#endif

bool running = true;
bool instaquit = false;
bool debugMode = false;
bool visualMode = false;
bool executionPaused = false;

void swap_buffers();
SDL_Surface *screen = NULL;
SDL_Surface *pausePicture = NULL;

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
	switch(info->info)
	{
		case 0:
			running = false;
			return 0;
		case 1:
			fprintf(stdout, "%c", info->input0);
			return 0;
		case 2:
			fprintf(stdout, "%d", info->input0);
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
  switch(info->info)
  {
    case 1: swap_buffers(); break;
		default:
			fprintf(
				stderr, 
				"HWIO [%d]: (%d, %d)\n", 
				info->info, 
				info->input0, 
				info->input1);
  }
	return 0;
}

spu_t mainCore;

void dump_vm()
{
	fprintf(stderr,
		"cp=%8X bp=%3d f=%1X [",
		mainCore.codePointer,
		mainCore.basePointer,
		mainCore.flags
	);

	for (int i = 0; i <= mainCore.stackPointer; i++)
	{
		fprintf(stderr," %d", mainCore.stack[i]);
	}

	fprintf(stderr," ]\n");
}

void update_vm()
{
	vm_step_process(&mainCore);

	// Debug mode is for live debugging,
	// paused execution is for manual debugging 
	if(debugMode || executionPaused) dump_vm();
}

void update_input(SDL_Event *ev)
{
	switch (ev->type)
	{
	case SDL_QUIT: 
    instaquit = true;
		running = false;
		break;
	case SDL_KEYDOWN:
		switch(ev->key.keysym.sym)
		{
			case SDLK_ESCAPE: running = false; return;
			case SDLK_F5:
				executionPaused = !executionPaused; 
				swap_buffers();
				return;
			case SDLK_F10: 
				if(executionPaused) {
					update_vm();
					swap_buffers();
				}
				return;
			case SDLK_1:
				mainCore.interrupt = 1;
				return;
			case SDLK_2:
				mainCore.interrupt = 2;
				return;
			case SDLK_3:
				mainCore.interrupt = 3;
				return;
			case SDLK_4:
				mainCore.interrupt = 4;
				return;
			case SDLK_5:
				mainCore.interrupt = 5;
				return;
		}
		break;
	}
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
	mainCore.flags = VM_FLAG_IE;
	
	mainCore.interrupt = 0;

	// Clear stack
	memset(mainCore.stack, 0, VM_STACKSIZE * sizeof(uint32_t));
}

bool load_exp(const char *fileName)
{
	FILE *f = fopen(fileName, "rb");
	if (f == NULL)
	{
		fprintf(stderr, "Could not open file %s\n", fileName);
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////

	expfile_t fileHeader;

	if (fread(&fileHeader, 1, sizeof(expfile_t), f) != sizeof(expfile_t))
	{
		fprintf(stderr, "File %s does not contain a valid header.\n", fileName);
		return false;
	}
	if (fileHeader.magicNumber != EXP_MAGIC)
	{
		fprintf(stderr, "Invalid magic in %s\n", fileName);
		return false;
	}
	if (fileHeader.majorVersion != 1 && fileHeader.minorVersion == 0)
	{
		fprintf(
			stderr, "Invalid version %s: %d.%d\n",
			fileName,
			fileHeader.majorVersion, fileHeader.minorVersion);
		return false;
	}

	for (uint32_t i = 0; i < fileHeader.numSections; i++)
	{
		expsection_t section;

		fseek(f, fileHeader.posSections + i * sizeof(expsection_t), SEEK_SET);
		fread(&section, 1, sizeof(expsection_t), f);
		
		fseek(f, section.start, SEEK_SET);

		uint8_t *sectionInRam = (uint8_t*)mainCore.memory + section.base;
		int len = fread(sectionInRam, 1, section.length, f);
		if (len != section.length)
			fprintf(stderr, "Read invalid size.\n");
	}

	return true;
}

int executionsPerSimulationStep = 50;
bool autoSwapBuffers = false;

void run_visual_mode()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		exit(1);
	}
	atexit(SDL_Quit);

	screen = SDL_SetVideoMode(640, 480, 32, SDL_DOUBLEBUF);
	SDL_WM_SetCaption("DasOS Virtual Platform", NULL);

	pausePicture = SDL_LoadBMP("pause.bmp");
	
	// Visual Mode starts with paused
	// execution for better visualization
	// technique
	executionPaused = true;
	
	swap_buffers();
	
	SDL_Event ev;
	while (running)
	{
		while (SDL_PollEvent(&ev))
		{
			update_input(&ev);
		}

		if(!executionPaused)
		{
			uint32_t start = SDL_GetTicks();
			do {
				for (int i = 0; i < executionsPerSimulationStep && running; i++)
				{
					update_vm();
				}
			} while (running && (SDL_GetTicks() - start) <= 32);
		}

    if(autoSwapBuffers) swap_buffers();
	}

  if(instaquit)
    return;
  
  // Draw the current VRAM state
  swap_buffers();
  
	SDL_WM_SetCaption("DasOS Virtual Platform - STOPPED", NULL);

	running = true;
	while (running)
	{
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
				running = false;
			else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)
				running = false;
		}

		SDL_Flip(screen);

		SDL_Delay(32);
	}
}

void swap_buffers()
{
  if(screen == NULL)
    return;
  SDL_LockSurface(screen);
  memcpy(
    screen->pixels,
    (uint8_t*)mainCore.memory + 4096,
    screen->h * screen->pitch);
  SDL_UnlockSurface(screen);
	
	if(executionPaused)
	{
		SDL_Rect target = {
			4, 4, 10, 24,
		};
		SDL_BlitSurface(
			pausePicture,
			NULL,
			screen,
			&target);
		target = (SDL_Rect){
			18, 4, 10, 24,
		};
		SDL_BlitSurface(
			pausePicture,
			NULL,
			screen,
			&target);
	}
 
  SDL_Flip(screen);
}

void run_text_mode()
{
	while (running)
	{
		//TODO: Insert some kind of text events.
		update_vm();
	}
}

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
      if(executionsPerSimulationStep <= 0)
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
  
  if(optind >= argc) {
    fprintf(stderr, "No initial RAM-file.\n");
    exit(1);
  }
  
	for (int index = optind; index < argc; index++)
	{
		// fprintf(stderr, "Loading %s...\n", argv[index]);
		if(load_exp(argv[index]) == 0) {
			fprintf(stderr, "%s not found.\n", argv[index]);
			exit(1);
		}
	}
	
	if(debugMode) dump_vm();
	
	if(visualMode)
		run_visual_mode();
	else
		run_text_mode();
	
	return 0;
}
