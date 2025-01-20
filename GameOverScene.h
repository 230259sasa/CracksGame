#pragma once
#include "Engine/GameObject.h"
class Sprite;
class GameOverScene :
    public GameObject
{
    Sprite* tex_;
public:
    GameOverScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

