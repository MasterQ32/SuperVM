#include "stub.h"

typedef devstub_t devlocal_t;

static void dev_update(devlocal_t *device)
{

}

static uint32_t dev_read(devlocal_t *device, uint16_t reg)
{
	switch(reg) {
		default: return -1;
	}
}

static void dev_write(devlocal_t *device, uint16_t reg, uint32_t value)
{
	switch(reg) {
	
		default: /* ??? */ break;
	}
}

static void serial_shutdown(devlocal_t *device)
{

}

device_t *devstub_create()
{
	devlocal_t *device = malloc(sizeof(devlocal_t));
	
	strcpy(serial->device.name, "Serial Port v1.0");
	serial->device.shutdown = (void(*)(device_t*))dev_shutdown;
	serial->device.update = (void(*)(device_t*))dev_update;
	serial->device.read = (uint32_t(*)(device_t*,uint16_t))dev_read;
	serial->device.write = (void(*)(device_t*, uint16_t, uint32_t))dev_write;

	
	
	return &device->device;
}