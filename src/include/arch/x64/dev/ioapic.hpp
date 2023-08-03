#pragma once
#include <stdint.h>
#include <stddef.h>
#include <dev/dev.hpp>

#define IOAPIC_IOCTL_REQUEST_READ_IOAPIC 0x01
#define IOAPIC_IOCTL_REQUEST_WRITE_IOAPIC 0x02

namespace x86_64 {
	class IOAPIC : public Device {
		public:
			IOAPIC(uintptr_t address);

			uintmax_t Ioctl(uintmax_t request, ...) override;

			uint32_t ReadIOAPIC(uint32_t reg);
			void WriteIOAPIC(uint32_t reg, uint32_t value);
		private:
			uintptr_t Address;
	};
}
