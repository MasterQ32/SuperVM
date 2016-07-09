#pragma once

#include <stdint.h>

struct device;

struct device
{
	/**** Generic Device Information ****/
	// Name of the device
	char name[128];

	/**** Interaction API ****/
	
	// Write to a register
	void (*write)(struct device *device, uint16_t reg, uint32_t value); 
	
	// Read from a register
	uint32_t(*read)(struct device *device, uint16_t reg);
	
	// Update the device.
	void(*update)(struct device *device);
	
	// Shuts down the device
	void(*shutdown)(struct device *device);

	union {
		void *tagPtr;
		uint32_t tagValue;
	};
};

// To be called when a device will emit an interrupt.
// device:    The device that triggered the interrupt
// interrupt: The local interrupt the device triggered
void device_interrupt_callback(struct device *device, uint32_t interrupt);

typedef struct device device_t;
