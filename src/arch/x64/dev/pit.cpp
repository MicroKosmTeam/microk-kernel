#include <arch/x64/dev/pit.hpp>
#include <arch/x64/io/io.hpp>

#ifdef UNDEF

#define PIT_IO_PORT_CHANNEL_0        0x40 /* Read and write */
#define PIT_IO_PORT_CHANNEL_1        0x41 /* Read and write */
#define PIT_IO_PORT_CHANNEL_2        0x42 /* Read and write */
#define PIT_IO_PORT_COMMAND_REGISTER 0x43 /* Write only */

namespace x86_64 {
PIT::PIT() {
}

uintmax_t PIT::Ioctl(uintmax_t request, ...) {
	va_list ap;
	va_start(ap, request);

	uintmax_t result;

	switch (request) {
		default:
			result = 0;
	}

	va_end(ap);

	return result;
}
}
#endif
