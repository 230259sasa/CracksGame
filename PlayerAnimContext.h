#pragma once
#include"Player.h"
#include<array>

class PlayerAnimPermission;

class PlayerAnimContext
{
	PlayerAnimPermission* animPermission_;
	std::array<PlayerAnimPermission*, Player::ANIM_TYPE_MAX> animPermissionArr_;
public:
	PlayerAnimContext(Player* _player);
	~PlayerAnimContext();
	void Change(Player::AnimType _type);
	Player::AnimType GetCurrentAnimType();
	std::array<bool, Player::FUNCTION_INDEX_MAX> Permission();
};

