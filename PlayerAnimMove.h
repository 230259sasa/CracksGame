#pragma once
#include "PlayerAnimPermission.h"
class PlayerAnimMove :
    public PlayerAnimPermission
{
public:
	PlayerAnimMove(Player* _p);
	Player::AnimType GetCurrentAnimType() override;
	std::array<bool, Player::FUNCTION_INDEX_MAX> GetIsFunctionArr() override;
};

