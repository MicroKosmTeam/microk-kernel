#pragma once
#include <cstdint.hpp>

extern "C" void SyscallMain(usize syscallNumber,
		            usize firstArgument,
		            usize secondArgument,
		            usize thirdArgument,
		            usize fourthArgument,
		            usize fithArgument,
			    usize sixthArgument);
