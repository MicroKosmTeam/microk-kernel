#pragma once
#include <cdefs.h>

extern "C" void SyscallMain(usize syscallNumber,
		            usize firstArgument,
		            usize secondArgument,
		            usize thirdArgument,
		            usize fourthArgument,
		            usize fithArgument,
			    usize sixthArgument);
