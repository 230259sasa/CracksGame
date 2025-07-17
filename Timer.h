#pragma once
#include "Engine\GameObject.h"
#include <chrono>

class Timer
{
	bool isRun_;
	float elapsed_;
	std::chrono::steady_clock::time_point startTime_;
public:
	Timer();
	bool IsTimeOver(float seconds);
	void ResetTimer();
	void StopTimer();
	void StartTimer();
	float GetSeconds();
};

