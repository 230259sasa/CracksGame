#pragma once
#include "Engine/GameObject.h"

class Texture;

class PlaySceneReady :
    public GameObject
{
    bool isReadyFinish_;
public:
    PlaySceneReady(GameObject* parent);
    ~PlaySceneReady();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
    bool IsReadyFinish();
};

