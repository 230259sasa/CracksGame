#pragma once
#include "Engine\GameObject.h"

class Sprite;

class PlayTimerBar :
    public GameObject
{
    bool isTimerRun_;
    float timer_;
    float initialTime_;
    Sprite* gaugeFrame_;
    Sprite* gauge_;
public:
    PlayTimerBar(GameObject* parent);
    ~PlayTimerBar();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
    void SetTimer(int _time) { timer_ = _time; initialTime_ = _time; }
    void TimerStop() { isTimerRun_ = false; }
    void TimerStart() { isTimerRun_ = true; }
    bool IsTimeOver() { return timer_ <= 0; }
};

