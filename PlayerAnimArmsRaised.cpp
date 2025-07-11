#include "PlayerAnimArmsRaised.h"

PlayerAnimArmsRaised::PlayerAnimArmsRaised(Player* _p)
    : PlayerAnimPermission(_p)
{
}

Player::AnimType PlayerAnimArmsRaised::GetCurrentAnimType()
{
    return Player::AnimType::STAND;
}

std::array<bool, Player::FUNCTION_INDEX_MAX> PlayerAnimArmsRaised::GetIsFunctionArr()
{
    for (int i = 0;i < (int)Player::FunctionIndex::MAX;i++) {
        isFunctionArr_[i] = true;
    }
    return isFunctionArr_;
}
