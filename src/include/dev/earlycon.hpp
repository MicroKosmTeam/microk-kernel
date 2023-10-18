#pragma once
#include <cdefs.h>
#include <cstdint.hpp>
#include <dev/uart.hpp>

namespace DEV::EARLYCON {
	Device *InitializeEarlycon(char *arguments);
}
