#pragma once
#include "FallObject.h"

class FallBomb :
    public FallObject
{
public:
    FallBomb(GameObject* _parent);
    ~FallBomb();
    void Initialize() override;
    void Release() override;
    void Update() override;
};

