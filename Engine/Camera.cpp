#include "Camera.h"

namespace Camera
{
	//変数
	XMVECTOR position_;	//カメラの位置（視点）
	XMVECTOR target_;	//見る位置（焦点）
	XMMATRIX viewMatrix_;	//ビュー行列
	XMMATRIX projMatrix_;	//プロジェクション行列
	Player* player_;
}


void Camera::Initialize()
{
	position_ = XMVectorSet(0, 7, -10, 0);	//カメラの位置
	target_ = XMVectorSet(0, 0, 0, 0);	//カメラの焦点
	player_ = nullptr;

	//プロジェクション行列
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)800 / (FLOAT)600, 0.1f, 100.0f);
}

void Camera::Initialize(XMVECTOR _position, XMVECTOR _target)
{
	position_ = _position;	//カメラの位置
	target_ = _target;	//カメラの焦点
	player_ = nullptr;

	//プロジェクション行列
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)800 / (FLOAT)600, 0.1f, 100.0f);
}

void Camera::Update()
{
	if (player_ != nullptr) {
		position_ = XMVectorSet(player_->GetPosition().x + 5,
			player_->GetPosition().y + 10,
			player_->GetPosition().z - 10,0);
		target_ = XMVectorSet(player_->GetPosition().x,
			player_->GetPosition().y,
			player_->GetPosition().z, 0);
	}
	//ビュー行列の作成
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));
}

void Camera::SetPosition(XMVECTOR _position)
{
	position_ = _position;
}

void Camera::SetTarget(XMVECTOR _target)
{
	target_ = _target;
}

void Camera::SetPlayerPointer(Player* _player)
{
	player_ = _player;
}

XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}

XMVECTOR Camera::GetPosition()
{
	return position_;
}

XMVECTOR Camera::GetTarget()
{
	return target_;
}
