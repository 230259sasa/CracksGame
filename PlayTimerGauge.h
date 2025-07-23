#pragma once
#include "Engine\GameObject.h"

class Sprite;

class PlayTimerGauge :
    public GameObject
{
    const XMFLOAT3 gauge_size_;
    const XMFLOAT3 gauge_frame_size_;
    const XMFLOAT3 gauge_position_;
    bool isTimerRun_;
    float timer_;
    float initialTime_;
    Sprite* gaugeFrame_;
    Sprite* gauge_;
public:
    PlayTimerGauge(GameObject* parent);
    ~PlayTimerGauge();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
    void SetTimer(int _time) { timer_ = _time; initialTime_ = _time; }
    void TimerStop() { isTimerRun_ = false; }
    void TimerStart() { isTimerRun_ = true; }
    bool IsTimeOver() { return timer_ <= 0; }
};

