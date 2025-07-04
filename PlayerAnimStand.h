#pragma once
#include "PlayerAnimPermission.h"
class PlayerAnimStand :
    public PlayerAnimPermission
{
public:
	PlayerAnimStand(Player* _p);
	Player::AnimType GetCurrentAnimType() override;
	std::array<bool, Player::FUNCTION_INDEX_MAX> GetIsFunctionArr() override;
};

