#include <sys/time.hpp>

namespace TIME {

int InitializeTimer(Timer *timer) {
	if (timer == NULL || timer->Initialize == NULL)
		return -EINVALID;

	return timer->Initialize(timer);
}

ticks_t GetTimerTicks(Timer *timer) {
	if (timer == NULL || timer->GetTicks == NULL)
		return -EINVALID;

	return timer->GetTicks(timer);
}
	
ticks_t GetTimerDivider(Timer *timer) {
	if (timer == NULL || timer->GetDivider == NULL)
		return -EINVALID;

	return timer->GetDivider(timer);
}

int WaitSync(Timer *timer, ticks_t ticks) {
	if (timer == NULL || timer->WaitSync == NULL)
		return -EINVALID;

	return timer->WaitSync(timer, ticks);
}

}
