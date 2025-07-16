#include "DeltaTime.h"
#include <Windows.h>

namespace DeltaTime {
	bool isRun_;
    double dTime_;
	DWORD pastTime(0.0);
	DWORD nowTime(0.0);
}

void DeltaTime::Initialize()
{
	nowTime = timeGetTime();
	pastTime = nowTime;
	isRun_ = true;
}

void DeltaTime::Update()
{
	timeBeginPeriod(1);
	pastTime = nowTime;
	nowTime = timeGetTime();
	timeEndPeriod(1);
}

double DeltaTime::GetDeltaTime()
{
	if(isRun_)
		return (nowTime-pastTime)/1000.0;

	return 0;
}

void DeltaTime::Stop()
{
	isRun_ = false;
}

void DeltaTime::Run()
{
	isRun_ = true;
}
