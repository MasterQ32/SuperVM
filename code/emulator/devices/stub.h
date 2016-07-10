#pragma once

#include "device.h"

#define STUB_REG_0 0
#define STUB_REG_1 1
#define STUB_REG_2 2

BEGIN_DEVICE(stub);
	int local;
END_DEVICE();
