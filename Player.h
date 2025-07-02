#pragma once
#include "Engine\GameObject.h"

class PlayerAnimContext;

class Player :
    public GameObject
{
public:
    enum class FunctionIndex {
        MOVE,
        JUMP,
        MAX
    };
    enum class AnimType {
        STAND = 0,
        MOVE,
        PUNCH,
        MAX
    };
    static constexpr int ANIM_TYPE_MAX = static_cast<int>(AnimType::MAX);
    static constexpr int FUNCTION_INDEX_MAX = static_cast<int>(FunctionIndex::MAX);
private:
    using AT = AnimType;
    using FI = FunctionIndex;
    enum CaneraDir {
        LEFT = -1,
        RIGHT = 1
    };
    struct AnimData {
        int hModel;
        int animFrameNum;
        bool isAnimAction;
    };
    AnimType animID_;
    AnimData animData_[ANIM_TYPE_MAX];
    bool isGround_;
    bool isFunctionEnabled_[FUNCTION_INDEX_MAX];
    float jumpVelocity_;
    XMINT3 framePos_;
    XMINT3 pastPos_;
    bool isCameraRotateStart_;
    int CameraRotateDir_;
    PlayerAnimContext* animContext_;
public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;

    bool GetIsAnimAction(AnimType _type);
private:
    void AnimationManager();
    void ActionManager();
    bool Move();
    void Jump();
    void Fall();
    void Relocate();
    void MoveCamera();
    void BreakStageBlock();
};

