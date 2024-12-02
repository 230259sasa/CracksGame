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
	//デバック用の移動

	//移動キーが押されてなければreturn
	if (!Input::IsKey(DIK_W) && !Input::IsKey(DIK_S) &&
		!Input::IsKey(DIK_A) && !Input::IsKey(DIK_D))
		return;
	
	//ベクトルを求める
	float vectorZ = XMVectorGetZ(Camera::GetTarget())
		- XMVectorGetZ(Camera::GetPosition());
	float vectorX = XMVectorGetX(Camera::GetTarget())
		- XMVectorGetX(Camera::GetPosition());
	float length = vectorX * vectorX + vectorZ * vectorZ;

	//normalize
	if(vectorX<0)
		vectorX = -(vectorX * vectorX) / length;
	else
		vectorX = (vectorX * vectorX) / length;

	if(vectorZ<0)
		vectorZ = -(vectorZ * vectorZ) / length;
	else
		vectorZ = (vectorZ * vectorZ) / length;

	//移動する方向を入力
	if (Input::IsKey(DIK_W)) {
	}
	if (Input::IsKey(DIK_S)) {
		vectorX *= -1;
		vectorZ *= -1;
	}
	if (Input::IsKey(DIK_A)) {
		float r = XMConvertToRadians(Set::LEFT_MOVE_ANGLE);
		float ix = vectorX * cos(r) - vectorZ * sin(r);
		float iz = vectorX * sin(r) + vectorZ * cos(r);
		vectorX = ix;
		vectorZ = iz;
	}
	if (Input::IsKey(DIK_D)) {
		float r = XMConvertToRadians(Set::RIGHT_MOVE_ANGLE);
		float x = vectorX * cos(r) - vectorZ * sin(r);
		float z = vectorX * sin(r) + vectorZ * cos(r);
		vectorX = x;
		vectorZ = z;
	}

	//移動
	transform_.position_.x += Set::MOVE_SPEED * vectorX;
	transform_.position_.z += Set::MOVE_SPEED * vectorZ;
}