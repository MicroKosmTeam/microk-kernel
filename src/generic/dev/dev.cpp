#include <dev/dev.hpp>

namespace DEV {
int InitializeDevice(Device *device, ...) {
	if (device == NULL || device->Initialize == NULL)
		return -EINVALID;

	va_list ap;
	va_start(ap, device);

	int result = device->Initialize(device, ap);

	va_end(ap);

	return result;
}

int DeinitializeDevice(Device *device, ...) {
	if (device == NULL || device->Deinitialize == NULL)
		return -EINVALID;

	va_list ap;
	va_start(ap, device);

	int result = device->Deinitialize(device, ap);

	va_end(ap);

	return result;
}

intmax_t Ioctl(Device *device, request_t request, ...) {
	if (device == NULL || device->Ioctl == NULL)
		return -EINVALID;

	va_list ap;
	va_start(ap, request);

	intmax_t result = device->Ioctl(device, request, ap);

	va_end(ap);

	return result;
}

}
