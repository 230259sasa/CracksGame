#pragma once
#include "FallObject.h"

class Timer;

class FallBomb :
    public FallObject
{
    Timer* explosionTimer_;
public:
    FallBomb(GameObject* _parent);
    ~FallBomb();
    void Initialize() override;
    void Release() override;
    void Update() override;
};

