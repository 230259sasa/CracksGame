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
        ARMS_RAISED,
        MAX
    };
    static constexpr int ANIM_TYPE_MAX = static_cast<int>(AnimType::MAX);
    static constexpr int FUNCTION_INDEX_MAX = static_cast<int>(FunctionIndex::MAX);
private:
    class FallObject;
    enum CaneraDir {
        LEFT = -1,
        RIGHT = 1
    };
    struct AnimData {
        int hModel;
        int animFrameNum;
        bool isAnimAction;
    };

    const float move_speed_;
    const float player_radius_;
    const float initial_jump_velocity_;
    const float max_fall_velocity_;

    FallObject* heldObject_;
    using AT = AnimType;
    using FI = FunctionIndex;
    
    AnimType animID_;
    AnimData animData_[ANIM_TYPE_MAX];
    bool isGround_;
    bool isCameraRotateStart_;
    bool isFunctionEnabled_[FUNCTION_INDEX_MAX];
    int CameraRotateDir_;
    float jumpVelocity_;
    XMINT3 framePos_;
    XMINT3 pastPos_;
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
    void Move();
    void Jump();
    void Fall();
    void Relocate();
    void MoveCamera();
    void BreakStageBlock();
};

