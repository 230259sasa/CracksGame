#pragma once
#include "Engine/GameObject.h"

class Sprite;
class Timer;

class PlaySceneReady :
    public GameObject
{
    Timer* timer_;
public:
    PlaySceneReady(GameObject* parent);
    ~PlaySceneReady();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
private:
    void Stop();
    void Start();
};

