#include "gpu.h"

#include "vm.h"
#include <stdbool.h>
#include <stdio.h>

typedef devgpu_t devlocal_t;

extern spu_t mainCore;
extern bool running;

static uint32_t dev_read(devlocal_t *device, uint16_t reg)
{
	switch(reg) {		
		case GPU_REG_MODE:   return -1; // TODO: Define mode table
		case GPU_REG_WIDTH:  return device->screen->w;
		case GPU_REG_HEIGHT: return device->screen->h;
		case GPU_REG_DEPTH:  return 32; // Assume 32 bpp
		case GPU_REG_LFBA:   return 0x8000;
		default: return -1;
	}
}

static void dev_write(devlocal_t *device, uint16_t reg, uint32_t value)
{

}

static void dev_update(devlocal_t *device)
{
	SDL_Event ev;
	SDL_PollEvent(&ev);
	if(ev.type == SDL_QUIT) {
		running = false;
	}
	
	if(clock() > device->nextRefresh) {
		/* copy memory here */
		SDL_LockSurface(device->screen);
		memcpy(
			device->screen->pixels,
			(uint8_t*)mainCore.memory + dev_read(device, GPU_REG_LFBA),
			device->screen->h * device->screen->pitch);
		SDL_UnlockSurface(device->screen);
		
		SDL_Flip(device->screen);
		device->nextRefresh = clock() + device->refreshTime;
	}
}

static void dev_shutdown(devlocal_t *device)
{

}

device_t *devgpu_create()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return NULL;
	}
	atexit(SDL_Quit);

	devlocal_t *device = malloc(sizeof(devlocal_t));
	
	strcpy(device->device.name, "vGPU 1.0");
	device->device.shutdown = (void(*)(device_t*))dev_shutdown;
	device->device.update = (void(*)(device_t*))dev_update;
	device->device.read = (uint32_t(*)(device_t*,uint16_t))dev_read;
	device->device.write = (void(*)(device_t*, uint16_t, uint32_t))dev_write;

	device->nextRefresh = clock();
	device->refreshTime = CLOCKS_PER_SEC / 30; // 30 FPS
	
	device->screen = SDL_SetVideoMode(640, 480, 32, SDL_DOUBLEBUF);
	SDL_WM_SetCaption("DasOS Virtual Platform", NULL);
	
	return &device->device;
}