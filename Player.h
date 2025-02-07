#pragma once
#include "Engine\GameObject.h"
class Player :
    public GameObject
{
    enum CaneraDir {
        LEFT = -1,
        RIGHT = 1
    };
    int hModel_;
    bool isGround_;
    float jumpVelocity_;
    XMINT3 framePos_;
    XMINT3 pastPos_;
    bool isCameraRotateStart_;
    int CameraRotateDir_;
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
    void Relocate();
    void MoveCamera();
    void SetBlock();
    void BreakStageBlock();
};

