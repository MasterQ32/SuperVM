#pragma once

#include "device.h"
#include <limits.h>

#define SERIAL_REG_DATA 0
#define SERIAL_REG_STATUS 1

#define SERIAL_BUFFER_SIZE 32

struct devserial
{
	device_t device;

	int sock;
	int remote;
	
	char sockName[PATH_MAX];
};

typedef struct devserial devserial_t;

device_t *devserial_create();