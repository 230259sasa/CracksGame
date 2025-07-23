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
    const float play_scene_ready_time_;
    const float start_image_position_y_;
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

