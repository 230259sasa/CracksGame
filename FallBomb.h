#pragma once
#include "FallObject.h"

class Timer;

class FallBomb :
    public FallObject
{
    const float explosion_time_;
    Timer* explosionTimer_;
public:
    FallBomb(GameObject* _parent);
    ~FallBomb();
    void Initialize() override;
    void Release() override;
    void Update() override;
private:
    void Explosion();
};

