#pragma once
#include "Engine\GameObject.h"
#include <chrono>

class Timer
{
	std::chrono::steady_clock::time_point startTime_;
public:
	Timer();
	bool IsTimeOver(float seconds);
	void ResetTimer();
	void Start();
};

