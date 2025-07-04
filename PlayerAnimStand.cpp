#include "PlayerAnimStand.h"

PlayerAnimStand::PlayerAnimStand(Player* _p)
    : PlayerAnimPermission(_p)
{
}

Player::AnimType PlayerAnimStand::GetCurrentAnimType()
{
    for (int i = (int)Player::AnimType::MAX-1;i >= 0;i--) {
        if (player_->GetIsAnimAction((Player::AnimType)i)) {
            return (Player::AnimType)i;
        }
    }
    return Player::AnimType::STAND;
}

std::array<bool, Player::FUNCTION_INDEX_MAX> PlayerAnimStand::GetIsFunctionArr()
{
    for (int i = 0;i < (int)Player::FunctionIndex::MAX;i++) {
        isFunctionArr_[i] = true;
    }
    return isFunctionArr_;
}
