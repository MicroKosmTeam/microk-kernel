#pragma once
#include <cdefs.h>
#include <cstdint.hpp>


typedef intmax_t time_t;
typedef intmax_t ticks_t;

namespace TIME {
	struct Timer {
		int (*Initialize)(Timer *timer);

		ticks_t (*GetTicks)(Timer *timer);
		ticks_t (*GetDivider)(Timer *timer);

		int (*WaitSync)(Timer *timer, ticks_t ticks);
	};

	/* Tells the timer to initialze itself */
	int InitializeTimer(Timer *timer);

	/* Returns the total amount of elapsed ticks */
	ticks_t GetTimerTicks(Timer *timer);
	
	/* Returns the number of ticks per second */
	ticks_t GetTimerDivider(Timer *timer);

	/* Waits for the time to expire */
	int WaitSync(Timer *timer, ticks_t ticks);
}
