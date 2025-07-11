#pragma once
#include "PlayerAnimPermission.h"
class PlayerAnimArmsRaised :
    public PlayerAnimPermission
{
public:
	PlayerAnimArmsRaised(Player* _p);
	Player::AnimType GetCurrentAnimType() override;
	std::array<bool, Player::FUNCTION_INDEX_MAX> GetIsFunctionArr() override;
};

