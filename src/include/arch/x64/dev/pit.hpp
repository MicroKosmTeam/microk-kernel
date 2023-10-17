#pragma once
#include <cstdint.hpp>

#include <dev/dev.hpp>

namespace x86_64 {
#ifdef UNDEF
	class PIT : public Device {
		public:
			PIT();
			umax Ioctl(umax request, ...) override;

		private:

	};
#endif
}
