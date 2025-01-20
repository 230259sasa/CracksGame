#pragma once
#include "Engine/GameObject.h"

class Sprite;

class TitleScene :
    public GameObject
{
    Sprite* tex_;
public:
    TitleScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

