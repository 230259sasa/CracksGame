#include "Timer.h"
#include "Engine\DeltaTime.h"

namespace DT = DeltaTime;

using namespace std::chrono;

Timer::Timer():startTime_(steady_clock::now()),elapsed_(0.0f),isRun_(true)
{
}

bool Timer::IsTimeOver(float seconds)
{
	if(isRun_)
		return elapsed_ + duration <float>(steady_clock::now() - startTime_).count() >= seconds;

	return false;
}

void Timer::ResetTimer()
{
	startTime_ = steady_clock::now();
	elapsed_ = 0.0f;
	isRun_ = true;
}

void Timer::StopTimer()
{
	if (isRun_) {
		elapsed_ += duration <float>(steady_clock::now() - startTime_).count();
		isRun_ = false;
	}
}

void Timer::StartTimer()
{
	if (!isRun_) {
		startTime_ = steady_clock::now();
		isRun_ = true;
	}
}

float Timer::GetSeconds()
{
	if(isRun_)
		return elapsed_ + duration <float>(steady_clock::now() - startTime_).count();

	return elapsed_;
}
