#pragma once
#include <cdefs.h>

extern "C" void HypercallMain(usize syscallNumber,
		            usize firstArgument,
		            usize secondArgument,
		            usize thirdArgument,
		            usize fourthArgument,
		            usize fithArgument,
			    usize sixthArgument);
