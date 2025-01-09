#pragma once
#include "Engine\GameObject.h"
class Player :
    public GameObject
{
    int hModel_;
    bool isGround_;
    float jumpVelocity_;
public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
private:
    void Move();
    void Jump();
    void Fall();
    void MoveCamera();
};

