#pragma once
#include <cstdint.hpp>
#include <dev/dev.hpp>

#define xAPIC_REGISTER_LAPIC_ID                            0x020
#define xAPIC_REGISTER_LAPIC_VERSION                       0x030
#define xAPIC_REGISTER_TASK_PRIORITY                       0x080
#define xAPIC_REGISTER_ARBITRATION_PRIORITY                0x090
#define xAPIC_REGISTER_PROCESSOR_PRIORITY                  0x0A0
#define xAPIC_REGISTER_EOI                                 0x0B0
#define xAPIC_REGISTER_REMOTE_READ                         0x0C0
#define xAPIC_REGISTER_LOGICAL_DESTINATION                 0x0D0
#define xAPIC_REGISTER_DESTINATION_FORMAT                  0x0E0
#define xAPIC_REGISTER_SPURIOUS_INTERRUPT_VECTOR           0x0F0
#define xAPIC_REGISTER_IN_SERVICE(n)                      (0x100 + n * 0x10)
#define xAPIC_REGISTER_IN_SERVICE_MAX_COUNT                8
#define xAPIC_REGISTER_TRIGGER_MODE(n)                    (0x180 + n * 0x10)
#define xAPIC_REGISTER_TRIGGER_MODE_MAX_COUNT              8
#define xAPIC_REGISTER_INTERRUPT_REQUEST(n)               (0x200 + n * 0x10)
#define xAPIC_REGISTER_INTERRUOT_REQUEST_MAX_COUNT         8
#define xAPIC_REGISTER_ERROR_STATUS                        0x280
#define xAPIC_REGISTER_INTERRUPT_COMMAND_REGISTER(n)      (0x300 + n * 0x10)
#define xAPIC_REGISTER_INTERRUPT_COMMAND_REGISTER_MAX      2
#define xAPIC_REGISTER_LVT_TIMER_REGISTER                  0x320
#define xAPIC_REGISTER_LVT_TERMAL_SENSOR                   0x330
#define xAPIC_REGISTER_LVT_PERFORMANCE_MONITORING_COUNTERS 0x340
#define xAPIC_REGISTER_LVT_LINT0                           0x350
#define xAPIC_REGISTER_LVT_LINT1                           0x360
#define xAPIC_REGISTER_LVT_ERROR                           0x370
#define xAPIC_REGISTER_TIMER_INITIAL_COUNT                 0x380
#define xAPIC_REGISTER_TIMER_CURRENT_COUNT                 0x390
#define xAPIC_REGISTER_TIMER_DIVIDE_CONFIGURATION          0x3E0

#define x2APIC_REGISTER_LAPIC_ID                            0x02
#define x2APIC_REGISTER_LAPIC_VERSION                       0x03
#define x2APIC_REGISTER_TASK_PRIORITY                       0x08
#define x2APIC_REGISTER_ARBITRATION_PRIORITY                0x09
#define x2APIC_REGISTER_PROCESSOR_PRIORITY                  0x0A
#define x2APIC_REGISTER_EOI                                 0x0B
#define x2APIC_REGISTER_REMOTE_READ                         0x0C
#define x2APIC_REGISTER_LOGICAL_DESTINATION                 0x0D
#define x2APIC_REGISTER_DESTINATION_FORMAT                  0x0E
#define x2APIC_REGISTER_SPURIOUS_INTERRUPT_VECTOR           0x0F
#define x2APIC_REGISTER_IN_SERVICE(n)                      (0x10 + n)
#define x2APIC_REGISTER_IN_SERVICE_MAX_COUNT                8
#define x2APIC_REGISTER_TRIGGER_MODE(n)                    (0x18 + n)
#define x2APIC_REGISTER_TRIGGER_MODE_MAX_COUNT              8
#define x2APIC_REGISTER_INTERRUPT_REQUEST(n)               (0x20 + n)
#define x2APIC_REGISTER_INTERRUOT_REQUEST_MAX_COUNT         8
#define x2APIC_REGISTER_ERROR_STATUS                        0x28
#define x2APIC_REGISTER_INTERRUPT_COMMAND_REGISTER          0x30 /* In x2APIC mode, all the MSR is used for 64 bits */
#define x2APIC_REGISTER_LVT_TIMER_REGISTER                  0x32
#define x2APIC_REGISTER_LVT_TERMAL_SENSOR                   0x33
#define x2APIC_REGISTER_LVT_PERFORMANCE_MONITORING_COUNTERS 0x34
#define x2APIC_REGISTER_LVT_LINT0                           0x35
#define x2APIC_REGISTER_LVT_LINT1                           0x36
#define x2APIC_REGISTER_LVT_ERROR                           0x37
#define x2APIC_REGISTER_TIMER_INITIAL_COUNT                 0x38
#define x2APIC_REGISTER_TIMER_CURRENT_COUNT                 0x39
#define x2APIC_REGISTER_TIMER_DIVIDE_CONFIGURATION          0x3E
#define x2APIC_REGISTER_SELF_IPI                            0x3F

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
		bool x2APICMode;
	};


	DEV::Device *CreateAPICDevice();

	int InitializeDevice(DEV::Device *device, va_list ap);
	int DeinitializeDevice(DEV::Device *device, va_list ap);
	intmax_t Ioctl(DEV::Device *device, request_t request, va_list ap);
}
