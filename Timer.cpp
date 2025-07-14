#include "Timer.h"
#include "Engine\DeltaTime.h"

namespace DT = DeltaTime;

using namespace std::chrono;

Timer::Timer():startTime_(steady_clock::now())
{
}

bool Timer::IsTimeOver(float seconds)
{
	if (duration <float>(steady_clock::now() - startTime_).count() > seconds)
		return true;
	return false;
}

void Timer::ResetTimer()
{
	startTime_ = steady_clock::now();
}
