#pragma once
#include "Engine\GameObject.h"

class Timer :
	public GameObject
{
	float time_;
	bool isRun_;
public:
	Timer(GameObject* parent,float _time);
	void Initialize() override;
	void Release() override;
	void Update() override;
	void Draw() override {};
	bool IsTimeOver(){ return (time_ < 0) ? true : false; }
	void SetTimer(float _time) { time_ = _time; }
	void Start() { isRun_ = true; }
	void Stop() { isRun_ = false; }
};

