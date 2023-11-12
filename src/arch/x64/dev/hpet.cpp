#include <arch/x64/dev/hpet.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <mm/vmm.hpp>

/* That would be 10ms */
#define FRACTIONAL_TIME_TO_WAIT 100

/* Function to calibrate the TSC using HPET */
int CalibrateTSCWithHPET(uptr hpetAddress, u64 *tscTicksPerSecond) {
	KInfo *info = GetInfo();

	VMM::MapPage(info->KernelVirtualSpace, hpetAddress, hpetAddress + info->HigherHalfMapping, VMM_FLAGS_KERNEL_DATA);
	hpetAddress += info->HigherHalfMapping;

	// Typecast the HPET address to access the registers
	volatile u64 *capabilities = (volatile u64*)hpetAddress;
	volatile u64 *configuration = (volatile u64*)(hpetAddress + 0x10);
	volatile u64 *counter = (volatile u64*)(hpetAddress + 0xF0);

	// Check if HPET is available (bit 0 of the capabilities register should be set)
	if ((*capabilities & 0x1) == 0) {
		return 0; // HPET not available
	}

	// Enable HPET (set bit 0 of the configuration register)
	*configuration |= 0x1;

	// Calculate the desired HPET timer resolution (e.g., 1 millisecond)
	u64 counterClkPeriod = *capabilities >> 32;
	if (counterClkPeriod > 0x05F5E100) {
		PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Invalid clk period\r\n");
		return 0;
	}
	
	u64 hpetTimerFrequency = 1000000000000000 / counterClkPeriod;
	u64 hpetTimeToPass = hpetTimerFrequency / FRACTIONAL_TIME_TO_WAIT;

	// Set the main HPET counter to 0 for starting the calibration
	*counter = 0;

	// Read the initial value of the TSC
	u64 tscStart = __builtin_ia32_rdtsc();

	// Wait for the desired HPET timer resolution to elapse
	while (*counter < hpetTimeToPass);

	// Read the final value of the TSC after the specified interval has passed
	u64 tscEnd = __builtin_ia32_rdtsc();

	// Disable HPET (clear bit 0 of the configuration register)
	*configuration &= ~0x1;

	// Calculate the TSC frequency in cycles per nanosecond
	u64 elapsedTSCCcycles = (tscEnd - tscStart) * FRACTIONAL_TIME_TO_WAIT;

	// Convert HPET time to microseconds and calculate the TSC frequency
	*tscTicksPerSecond = elapsedTSCCcycles;

	return 1; // Calibration successful
}
