#pragma once
#include "Engine\GameObject.h"
#include"Engine\Sprite.h"

class TestScene :
    public GameObject
{
    Sprite* tex;
public:
    TestScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

