#include "timer.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/* WARNING: Timer only works with small delta and limit values.              */
/* Too large delta will overflow the integer before limit check works.       */
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

void timer_update(devtimer_t *timer)
{
	// determine timer increment
	clock_t current = clock();
	clock_t delta = current - timer->last_tick;

	if(delta <= 0) {
		// nothing happened at all
		return;
	}
	timer->last_tick = current;

	timer->reg_counter += delta;

	// if enabled and we have an overflow
	if (timer->reg_limit > 0 && timer->reg_counter > timer->reg_limit)
	{
		device_interrupt_callback(&timer->device, TIMER_INTR_OVERFLOW);
		timer->reg_counter = 0;
	}
}

uint32_t timer_read(devtimer_t *timer, uint16_t reg)
{
	switch (reg)
	{
	case TIMER_REG_FREQUENCY: return CLOCKS_PER_SEC; // this is our fixed frequency
	case TIMER_REG_COUNTER: return timer->reg_counter; 
	case TIMER_REG_LIMIT: return timer->reg_limit;
	default:
		return -1;
	}
}

void timer_write(devtimer_t *timer, uint16_t reg, uint32_t value)
{
	switch (reg)
	{
	case TIMER_REG_COUNTER: timer->reg_counter = value; break;
	case TIMER_REG_LIMIT: timer->reg_limit = value; break;
	}
}

device_t *devtimer_create()
{
	devtimer_t *timer = malloc(sizeof(devtimer_t));
	strcpy(timer->device.name, "Timer v1.0");

	timer->device.update = (void(*)(device_t*))timer_update;
	timer->device.shutdown = NULL;
	timer->device.read = (uint32_t(*)(device_t*,uint16_t))timer_read;
	timer->device.write = (void(*)(device_t*, uint16_t, uint32_t))timer_write;

	timer->last_tick = clock();
	timer->reg_counter = 0;
	timer->reg_limit = 0;

	return &timer->device;
}