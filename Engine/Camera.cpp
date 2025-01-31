#include "Camera.h"

namespace Camera
{
	//変数
	XMVECTOR position_;	//カメラの位置（視点）
	XMVECTOR target_;	//見る位置（焦点）
	XMMATRIX viewMatrix_;	//ビュー行列
	XMMATRIX projMatrix_;	//プロジェクション行列
	Player* player_;
	XMFLOAT3 cpos;
	const XMFLOAT3 CAMERA_POS(0, 10, -10);
	int rotate_;
}


void Camera::Initialize()
{
	position_ = XMVectorSet(0, 7, -10, 0);	//カメラの位置
	target_ = XMVectorSet(0, 0, 0, 0);	//カメラの焦点
	cpos = CAMERA_POS;
	rotate_ = 0;
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
		position_ = XMVectorSet(player_->GetPosition().x + cpos.x,
			player_->GetPosition().y + cpos.y,
			player_->GetPosition().z + cpos.z,0);
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

int Camera::GetRotate()
{
	return rotate_;
}
void Camera::RotateCameraLeft(int _angle)
{
	float r = XMConvertToRadians(360-_angle);
	rotate_ -= _angle;
	cpos.x = cpos.x * cos(r) - cpos.z * sin(r);
	cpos.z = cpos.x * sin(r) + cpos.z * cos(r);
}

void Camera::RotateCameraRight(int _angle)
{
	float r = XMConvertToRadians(_angle);
	rotate_ += _angle;
	cpos.x = cpos.x * cos(r) - cpos.z * sin(r);
	cpos.z = cpos.x * sin(r) + cpos.z * cos(r);
}
