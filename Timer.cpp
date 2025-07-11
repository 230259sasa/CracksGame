#include "Timer.h"
#include "Engine\DeltaTime.h"

namespace DT = DeltaTime;

Timer::Timer(GameObject* parent,float _time)
	:GameObject(parent,"Timer"),time_(_time),isRun_(false)
{
}

void Timer::Initialize()
{
}

void Timer::Release()
{
}

void Timer::Update()
{
	if (!isRun_ || time_ < 0.0f)
		return;

	time_ -= DT::GetDeltaTime();
}