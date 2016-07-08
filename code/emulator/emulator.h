#pragma once

#include "vm.h"
#include "devices/device.h"
#include <stdbool.h>

struct deviceconfig
{
	uint32_t interruptBase;
};

typedef struct deviceconfig deviceconfig_t;


#define DEVICE_COUNT 32
extern device_t *devices[DEVICE_COUNT];
extern deviceconfig_t deviceConfigs[DEVICE_COUNT];

extern bool running;
extern bool executionPaused;

extern spu_t mainCore;

void run_text_mode();
void run_visual_mode();

bool load_exp(const char *fileName);


void update_vm();