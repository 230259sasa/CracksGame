#pragma once
#include "Engine\GameObject.h"

class Timer
{
	float time_;
	bool isRun_;
public:
	Timer(float _time);
	Timer();
	void Update();
	bool IsTimeOver(){ return (time_ < 0) ? true : false; }
	void SetTimer(float _time) { time_ = _time; }
	void Start() { isRun_ = true; }
	void Stop() { isRun_ = false; }
};

