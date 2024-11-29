#include "Player.h"
#include"Engine\Model.h"
#include"Engine\Input.h"
#include"Engine\Camera.h"
#include<numbers>

namespace Set {
	const float PI(3.14);
	const float MOVE_SPEED(0.1f);
	const int LEFT_MOVE_ANGLE(90);
	const int RIGHT_MOVE_ANGLE(270);
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_(-1)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Assets/Model/BoxBrick.fbx");
	assert(hModel_ >= 0);
	//Stageのブロックに重ならないために足している
	transform_.position_.y += 1;
}

void Player::Release()
{
}

void Player::Update()
{
	Move();

	//カメラの位置を決める
	XMVECTOR pos = XMVectorSet(transform_.position_.x,
		transform_.position_.y, transform_.position_.z, 0);
	Camera::SetTarget(pos);
	pos += XMVectorSet(8, 7, -10, 0);
	Camera::SetPosition(pos);
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Move()
{
	//移動キーが押されてなければreturn
	if (!Input::IsKey(DIK_W) && !Input::IsKey(DIK_S) &&
		!Input::IsKey(DIK_A) && !Input::IsKey(DIK_D))
		return;

	//ベクトルを求める
	float z = XMVectorGetZ(Camera::GetTarget())
		- XMVectorGetZ(Camera::GetPosition());
	float x = XMVectorGetX(Camera::GetTarget())
		- XMVectorGetX(Camera::GetPosition());
	float len = x * x + z * z;
	//normalize
	if(x<0)
		x = -(x * x) / len;
	else
		x = (x * x) / len;

	if(z<0)
		z = -(z * z) / len;
	else
		z = (z * z) / len;

	if (Input::IsKey(DIK_W)) {
	}
	if (Input::IsKey(DIK_S)) {
		x *= -1;
		z *= -1;
	}
	if (Input::IsKey(DIK_A)) {
		float r = AngleToRadians(Set::LEFT_MOVE_ANGLE);
		float ix = x * cos(r) - z * sin(r);
		float iz = x * sin(r) + z * cos(r);
		x = ix;
		z = iz;
	}
	if (Input::IsKey(DIK_D)) {
		float r = AngleToRadians(Set::RIGHT_MOVE_ANGLE);
		float ix = x * cos(r) - z * sin(r);
		float iz = x * sin(r) + z * cos(r);
		x = ix;
		z = iz;
	}
	transform_.position_.x += Set::MOVE_SPEED * x;
	transform_.position_.z += Set::MOVE_SPEED * z;
}

float Player::AngleToRadians(float _angle)
{
	return (_angle * Set::PI)/180;
}
