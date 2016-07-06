#pragma once

#include "device.h"
#include <time.h>

#define TIMER_REG_FREQUENCY 0
#define TIMER_REG_COUNTER 1
#define TIMER_REG_LIMIT 2

#define TIMER_INTR_OVERFLOW 0

struct timer
{
	device_t device;

	clock_t last_tick;
	uint32_t reg_counter;
	uint32_t reg_limit;
};

typedef struct timer timer_t;

device_t *timer_create();