#include "DeltaTime.h"
#include <Windows.h>

namespace DeltaTime {
    double dTime_;
	DWORD pastTime(0.0);
	DWORD nowTime(0.0);
}

void DeltaTime::Initialize()
{
	nowTime = timeGetTime();
	pastTime = nowTime;
}

void DeltaTime::Update()
{
	timeBeginPeriod(1);
	pastTime = nowTime;
	nowTime = timeGetTime();
	timeEndPeriod(1);
}

double DeltaTime::GetDelaTime()
{
    return (nowTime-pastTime)/1000.0;
}
