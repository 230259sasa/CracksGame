#pragma once
#include "Engine/GameObject.h"

class Sprite;
class Timer;

class PlaySceneReady :
    public GameObject
{
    enum COUNT {
        START,
        ONE,
        TWO,
        TREE,
        MAX
    };
    int count_;
    Timer* timer_;
    Sprite* tex_;
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
    void UpDownImage(Transform& _trans);
};

