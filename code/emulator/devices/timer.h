#pragma once

#include "device.h"
#include <time.h>

#define TIMER_REG_FREQUENCY 0
#define TIMER_REG_COUNTER 1
#define TIMER_REG_LIMIT 2

#define TIMER_INTR_OVERFLOW 0

struct devtimer
{
	device_t device;

	clock_t last_tick;
	uint32_t reg_counter;
	uint32_t reg_limit;
};

typedef struct devtimer devtimer_t;

device_t *devtimer_create();