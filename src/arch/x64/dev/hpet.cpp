#include <arch/x64/dev/hpet.hpp>

// Structure representing the HPET registers
struct HPETRegisters {
	uint64_t Capabilities;    // HPET capabilities register
	uint64_t Configuration;  // HPET configuration register
	uint64_t Counter;        // HPET counter value register
				 // Add other HPET registers here if necessary
};

/* Function to calibrate the TSC using HPET */
int CalibrateTSCWith(void *hpetAddress, uint64_t *tscCyclesPerMicrosecond) {
	// Typecast the HPET address to access the registers
	volatile HPETRegisters *hpet = (HPETRegisters*)hpetAddress;

	// Check if HPET is available (bit 0 of the capabilities register should be set)
	if ((hpet->Capabilities & 0x1) == 0) {
		return 0; // HPET not available
	}

	// Enable HPET (set bit 0 of the configuration register)
	hpet->Configuration |= 0x1;

	// Calculate the desired HPET timer resolution (e.g., 1 millisecond)
	uint64_t hpetTimerResolution = 1000000000; // 1 microsecond in femtoseconds

	// Set the main HPET counter to 0 for starting the calibration
	hpet->Counter = 0;

	// Read the initial value of the TSC
	uint64_t tscStart = __builtin_ia32_rdtsc();

	// Wait for the desired HPET timer resolution to elapse
	while (hpet->Counter < hpetTimerResolution) {
		// Wait
	}

	// Read the final value of the TSC after the specified interval has passed
	uint64_t tscEnd = __builtin_ia32_rdtsc();

	// Disable HPET (clear bit 0 of the configuration register)
	hpet->Configuration &= ~0x1;

	// Calculate the TSC frequency in cycles per nanosecond
	uint64_t elapsedHPETTime = hpet->Counter;
	uint64_t elapsedTSCCcycles = tscEnd - tscStart;

	// Convert HPET time to microseconds and calculate the TSC frequency
	*tscCyclesPerMicrosecond = (elapsedTSCCcycles * 1000) / elapsedHPETTime;

	return 1; // Calibration successful
}
