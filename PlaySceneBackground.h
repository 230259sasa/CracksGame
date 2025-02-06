#pragma once
#include "Engine/GameObject.h"
class PlaySceneBackground :
    public GameObject
{
    int hModel_;
public:
    PlaySceneBackground(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

