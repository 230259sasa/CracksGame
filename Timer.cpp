#include "Timer.h"
#include "Engine\DeltaTime.h"

namespace DT = DeltaTime;

Timer::Timer(float _time):time_(_time),isRun_(false)
{
}

Timer::Timer():time_(0), isRun_(false)
{
}

void Timer::Update()
{
	if (!isRun_ || time_ < 0.0f)
		return;

	time_ -= DT::GetDeltaTime();
}