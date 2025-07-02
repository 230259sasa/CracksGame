#pragma once
#include"Player.h"
#include<array>

class PlayerAnimPermission
{
protected:
	Player* player_;
	Player::FunctionIndex function_;
	std::array<bool, Player::FUNCTION_INDEX_MAX> isFunctionArr_;
public:
	PlayerAnimPermission(Player* _p) { player_ = _p; }
	~PlayerAnimPermission() {};
	virtual Player::AnimType GetCurrentAnimType() = 0;
	virtual std::array<bool, Player::FUNCTION_INDEX_MAX> Permission() = 0;
};

