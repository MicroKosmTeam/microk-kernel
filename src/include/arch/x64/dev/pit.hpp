#pragma once
#include <stdint.h>
#include <stddef.h>
#include <dev/dev.hpp>

namespace x86_64 {
	class PIT : public Device {
		public:
			PIT();
			uintmax_t Ioctl(uintmax_t request, ...) override;

		private:

	};
}
