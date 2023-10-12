#include <dev/dev.hpp>

namespace DEV {
int InitializeDevice(Device *device) {
	if (device == NULL || device->Ioctl == NULL)
		return -EINVALID;

	return device->Initialize(device);
}

int DeinitializeDevice(Device *device) {
	if (device == NULL || device->Ioctl == NULL)
		return -EINVALID;

	return device->Deinitialize(device);
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
