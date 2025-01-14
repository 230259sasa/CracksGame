#include "Camera.h"

namespace Camera
{
	//�ϐ�
	XMVECTOR position_;	//�J�����̈ʒu�i���_�j
	XMVECTOR target_;	//����ʒu�i�œ_�j
	XMMATRIX viewMatrix_;	//�r���[�s��
	XMMATRIX projMatrix_;	//�v���W�F�N�V�����s��
	Player* player_;
	XMFLOAT3 cpos;
}


void Camera::Initialize()
{
	position_ = XMVectorSet(0, 7, -10, 0);	//�J�����̈ʒu
	target_ = XMVectorSet(0, 0, 0, 0);	//�J�����̏œ_
	cpos = XMFLOAT3(0, 6, -1);
	player_ = nullptr;

	//�v���W�F�N�V�����s��
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)800 / (FLOAT)600, 0.1f, 100.0f);
}

void Camera::Initialize(XMVECTOR _position, XMVECTOR _target)
{
	position_ = _position;	//�J�����̈ʒu
	target_ = _target;	//�J�����̏œ_
	player_ = nullptr;

	//�v���W�F�N�V�����s��
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
	//�r���[�s��̍쐬
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

void Camera::RotateCameraLeft()
{
	constexpr float r = XMConvertToRadians(1);
	cpos.z = cpos.z * cos(r) - cpos.x * sin(r);
	cpos.x = cpos.z * sin(r) + cpos.x * cos(r);
}

void Camera::RotateCameraRight()
{
	constexpr float r = XMConvertToRadians(1);
	cpos.x = cpos.x * cos(r) - cpos.z * sin(r);
	cpos.z = cpos.x * sin(r) + cpos.z * cos(r);
}
