#pragma once
#include <stdint.h>
#include <stddef.h>
#include <dev/dev.hpp>

namespace x86_64 {
#ifdef UNDEF
	class PIT : public Device {
		public:
			PIT();
			uintmax_t Ioctl(uintmax_t request, ...) override;

		private:

	};
#endif
}
