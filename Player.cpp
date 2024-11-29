#include "Player.h"
#include"Engine\Model.h"
#include"Engine\Input.h"

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_(-1)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("");
	assert(hModel_ >= 0);
}

void Player::Release()
{
}

void Player::Update()
{
}

void Player::Draw()
{
}
