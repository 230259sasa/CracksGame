#pragma once
#include "Engine/GameObject.h"

class Sprite;

class PlayStateCheck :
    public GameObject
{
    Sprite* number_[10];
public:
    PlayStateCheck(GameObject* parent);
    ~PlayStateCheck();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    bool IsClear();
    bool IsGameOver();
};

