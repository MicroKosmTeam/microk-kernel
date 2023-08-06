#include <arch/x64/dev/hpet.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <mm/vmm.hpp>

#define FRACTIONAL_TIME_TO_WAIT 0x100

/* Function to calibrate the TSC using HPET */
int CalibrateTSCWithHPET(uintptr_t hpetAddress, uint64_t *tscTicksPerSecond) {
	KInfo *info = GetInfo();

	VMM::MapMemory(info->kernelVirtualSpace, hpetAddress, hpetAddress);

	// Typecast the HPET address to access the registers
	volatile uint64_t *capabilities = hpetAddress;
	volatile uint64_t *configuration = hpetAddress + 0x10;
	volatile uint64_t *counter = hpetAddress + 0xF0;

	// Check if HPET is available (bit 0 of the capabilities register should be set)
	if ((*capabilities & 0x1) == 0) {
		return 0; // HPET not available
	}

	// Enable HPET (set bit 0 of the configuration register)
	*configuration |= 0x1;

	// Calculate the desired HPET timer resolution (e.g., 1 millisecond)
	uint64_t counterClkPeriod = *capabilities >> 32;
	if (counterClkPeriod > 0x05F5E100) {
		PRINTK::PrintK("Invalid clk period\r\n");
		return 0;
	}
	
	uint64_t hpetTimerFrequency = 1000000000000000 / counterClkPeriod;
	uint64_t hpetTimeToPass = hpetTimerFrequency / FRACTIONAL_TIME_TO_WAIT;

	// Set the main HPET counter to 0 for starting the calibration
	*counter = 0;

	// Read the initial value of the TSC
	uint64_t tscStart = __builtin_ia32_rdtsc();

	// Wait for the desired HPET timer resolution to elapse
	while (*counter < hpetTimeToPass);

	// Read the final value of the TSC after the specified interval has passed
	uint64_t tscEnd = __builtin_ia32_rdtsc();

	// Disable HPET (clear bit 0 of the configuration register)
	*configuration &= ~0x1;

	// Calculate the TSC frequency in cycles per nanosecond
	uint64_t elapsedTSCCcycles = (tscEnd - tscStart) * FRACTIONAL_TIME_TO_WAIT;

	// Convert HPET time to microseconds and calculate the TSC frequency
	*tscTicksPerSecond = elapsedTSCCcycles;

	return 1; // Calibration successful
}
