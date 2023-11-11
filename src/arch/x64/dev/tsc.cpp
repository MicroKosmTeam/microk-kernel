#include <arch/x64/dev/tsc.hpp>

#include <sys/printk.hpp>

namespace x86_64::TSC {
DEV::Device *CreateTSCDevice() {
	TSC *tsc = new TSC;

	tsc->Initialize = InitializeDevice;
	tsc->Deinitialize = DeinitializeDevice;
	tsc->Ioctl = Ioctl;

	return (DEV::Device*)tsc;
}

int InitializeDevice(DEV::Device *device, va_list ap) {
	TSC *tsc = (TSC*)device;
	tsc->TickRate = va_arg(ap, usize);
	tsc->MilliTickRate = tsc->TickRate / 1000;
	tsc->MicroTickRate = tsc->TickRate / 1000000;
	tsc->NanoTickRate = tsc->TickRate / 1000000000;
	
	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "TSC initialized with %d ticks per second.\r\n", tsc->TickRate);

	return 0;
}

int DeinitializeDevice(DEV::Device *device, va_list ap) {
	(void)device;
	(void)ap;

	return 0;
}


intmax_t Ioctl(DEV::Device *device, request_t request, va_list ap) {
	(void)device;
	(void)request;
	(void)ap;

	return 0;
}

void NDelay(TSC *tsc, usize nanoseconds) {
	usize start = RDTSC();
	usize end = start + tsc->NanoTickRate * nanoseconds;

	while(RDTSC() < end);
}

void UDelay(TSC *tsc, usize microseconds) {
	usize start = RDTSC();
	usize end = start + tsc->MicroTickRate * microseconds;

	while(RDTSC() < end);
}

void MDelay(TSC *tsc, usize milliseconds) {
	usize start = RDTSC();
	usize end = start + tsc->MilliTickRate * milliseconds;

	while(RDTSC() < end);
}

void Delay(TSC *tsc, usize seconds) {
	usize start = RDTSC();
	usize end = start + tsc->TickRate * seconds;

	while(RDTSC() < end);
}


}
