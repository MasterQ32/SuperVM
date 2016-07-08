#if defined(_MSC_VER)
#include <SDL.h>
#undef main
#else
#include <SDL/SDL.h>
#endif
#include <stdbool.h>

#include "emulator.h"

void swap_buffers();
SDL_Surface *screen = NULL;
SDL_Surface *pausePicture = NULL;

bool instaquit = false;

int executionsPerSimulationStep = 50;
bool autoSwapBuffers = false;

void update_input(SDL_Event *ev)
{
	switch (ev->type)
	{
	case SDL_QUIT:
		instaquit = true;
		running = false;
		break;
	case SDL_KEYDOWN:
		switch (ev->key.keysym.sym)
		{
		case SDLK_ESCAPE: running = false; return;
		case SDLK_F5:
			executionPaused = !executionPaused;
			swap_buffers();
			return;
		case SDLK_F10:
			if (executionPaused) {
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

		if (!executionPaused)
		{
			uint32_t start = SDL_GetTicks();
			do {
				for (int i = 0; i < executionsPerSimulationStep && running; i++)
				{
					update_vm();
				}
			} while (running && (SDL_GetTicks() - start) <= 32);
		}

		if (autoSwapBuffers) swap_buffers();
	}

	if (instaquit)
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
	if (screen == NULL)
		return;
	SDL_LockSurface(screen);
	memcpy(
		screen->pixels,
		(uint8_t*)mainCore.memory + 4096,
		screen->h * screen->pitch);
	SDL_UnlockSurface(screen);

	if (executionPaused)
	{
		SDL_Rect target = {
			4, 4, 10, 24,
		};
		SDL_BlitSurface(
			pausePicture,
			NULL,
			screen,
			&target);
		target = (SDL_Rect) {
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