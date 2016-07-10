#pragma once

#include "device.h"

#include <time.h>
#include <SDL/SDL.h>

#define GPU_REG_MODE   0
#define GPU_REG_WIDTH  1
#define GPU_REG_HEIGHT 2
#define GPU_REG_DEPTH  3
#define GPU_REG_LFBA   4

BEGIN_DEVICE(gpu);
	SDL_Surface *screen;
	clock_t nextRefresh;
	clock_t refreshTime;
END_DEVICE();
