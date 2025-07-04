#include "PlayerAnimContext.h"
#include "PlayerAnimStand.h"
#include "PlayerAnimMove.h"

PlayerAnimContext::PlayerAnimContext(Player* _player)
{
    animPermissionArr_[(int)Player::AnimType::STAND] = new PlayerAnimStand(_player);
    animPermissionArr_[(int)Player::AnimType::MOVE] = new PlayerAnimMove(_player);

    animPermission_ = animPermissionArr_[(int)Player::AnimType::STAND];
}

PlayerAnimContext::~PlayerAnimContext()
{
}

void PlayerAnimContext::ChangeAnimType(Player::AnimType _type)
{
    animPermission_ = animPermissionArr_[(int)_type];
}

Player::AnimType PlayerAnimContext::GetCurrentAnimType()
{
    return animPermission_->GetCurrentAnimType();
}

std::array<bool, Player::FUNCTION_INDEX_MAX> PlayerAnimContext::GetIsFunctionArr()
{
    return animPermission_->GetIsFunctionArr();
}
