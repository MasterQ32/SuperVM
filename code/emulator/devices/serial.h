#pragma once

#include "device.h"
#include <limits.h>

#define SERIAL_REG_DATA 0
#define SERIAL_REG_STATUS 1

// set or unset
#define USE_UNIX_SOCKET

struct devserial
{
	device_t device;

	int sock;
	int remote;
	
#if defined(USE_UNIX_SOCKET)
	char fileName[PATH_MAX];
#endif
};

typedef struct devserial devserial_t;

device_t *devserial_create(int portNum);