#pragma once
#include "Engine/GameObject.h"

class Sprite;

class PlayStateCheck :
    public GameObject
{
public:
    PlayStateCheck(GameObject* parent);
    ~PlayStateCheck();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    bool IsReady();
    bool IsClear();
    bool IsGameOver();
};

