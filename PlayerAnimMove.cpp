#include "PlayerAnimMove.h"

PlayerAnimMove::PlayerAnimMove(Player* _p)
    : PlayerAnimPermission(_p)
{
}

Player::AnimType PlayerAnimMove::GetCurrentAnimType()
{
    if (player_->GetIsAnimAction(Player::AnimType::MOVE))
        return Player::AnimType::MOVE;
    return Player::AnimType::STAND;
}

std::array<bool, Player::FUNCTION_INDEX_MAX> PlayerAnimMove::Permission()
{
    for (int i = 0;i < (int)Player::FunctionIndex::MAX;i++) {
        isFunctionArr_[i] = true;
    }
    return isFunctionArr_;
}
