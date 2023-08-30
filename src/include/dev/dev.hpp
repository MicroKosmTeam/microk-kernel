#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

class Device {
public:
	virtual ~Device();
	virtual uintmax_t Ioctl(uintmax_t request, ...) = 0;
protected:
};
