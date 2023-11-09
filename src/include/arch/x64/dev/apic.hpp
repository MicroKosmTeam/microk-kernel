#pragma once
#include <cstdint.hpp>
#include <dev/dev.hpp>

#define APIC_REGISTER_LAPIC_ID                            0x020
#define APIC_REGISTER_LAPIC_VERSION                       0x030
#define APIC_REGISTER_TASK_PRIORITY                       0x080
#define APIC_REGISTER_ARBITRATION_PRIORITY                0x090
#define APIC_REGISTER_PROCESSOR_PRIORITY                  0x0A0
#define APIC_REGISTER_EOI                                 0x0B0
#define APIC_REGISTER_REMOTE_READ                         0x0C0
#define APIC_REGISTER_LOGICAL_DESTINATION                 0x0D0
#define APIC_REGISTER_DESTINATION_FORMAT                  0x0E0
#define APIC_REGISTER_SPURIOUS_INTERRUPT_VECTOR           0x0F0
#define APIC_REGISTER_IN_SERVICE(n)                      (0x100 + n * 0x10)
#define APIC_REGISTER_IN_SERVICE_MAX_COUNT                8
#define APIC_REGISTER_TRIGGER_MODE(n)                    (0x180 + n * 0x10)
#define APIC_REGISTER_TRIGGER_MODE_MAX_COUNT              8
#define APIC_REGISTER_INTERRUPT_REQUEST(n)               (0x200 + n * 0x10)
#define APIC_REGISTER_INTERRUOT_REQUEST_MAX_COUNT         8
#define APIC_REGISTER_ERROR_STATUS                        0x280
#define APIC_REGISTER_LVT_CMCI                            0x2F0
#define APIC_REGISTER_INTERRUPT_COMMAND_REGISTER(n)      (0x300 + n * 0x10)
#define APIC_REGISTER_INTERRUPT_COMMAND_REGISTER_MAX      2
#define APIC_REGISTER_LVT_TIMER_REGISTER                  0x320
#define APIC_REGISTER_LVT_TERMAL_SENSOR                   0x330
#define APIC_REGISTER_LVT_PERFORMANCE_MONITORING_COUNTERS 0x340
#define APIC_REGISTER_LVT_LINT0                           0x350
#define APIC_REGISTER_LVT_LINT1                           0x360
#define APIC_REGISTER_LVT_ERROR                           0x370
#define APIC_REGISTER_TIMER_INITIAL_COUNT                 0x380
#define APIC_REGISTER_TIMER_CURRENT_COUNT                 0x390
#define APIC_REGISTER_TIMER_DIVIDE_CONFIGURATION          0x3E0

#define APIC_LVT_TIMER_MASK        0x10000
#define APIC_LVT_TIMER_DIVIDE_1    0x3
#define APIC_LVT_TIMER_ONESHOT     (0 << 17)
#define APIC_LVT_TIMER_PERIODIC    (1 << 17)
#define APIC_LVT_TIMER_TSCDEADLINE (2 << 17)

namespace x86_64::APIC {
	struct APIC : public DEV::Device {
		u32 ID;

		uptr Base;
		uptr MappedAddress;

		bool ProcessorIsBSP;
	};


	DEV::Device *CreateAPICDevice();

	int InitializeDevice(DEV::Device *device, va_list ap);
	int DeinitializeDevice(DEV::Device *device, va_list ap);
	intmax_t Ioctl(DEV::Device *device, request_t request, va_list ap);

	int ReadAPIC(APIC *device, usize registerSelector, u32 *value);
	int WriteAPIC(APIC *device, usize registerSelector, u32 value);
}
