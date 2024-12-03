#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "../Player.h"

using namespace DirectX;

//-----------------------------------------------------------
//�J����
//-----------------------------------------------------------
namespace Camera
{
	//�������i�v���W�F�N�V�����s��쐬�j
	void Initialize();
	void Initialize(XMVECTOR _position, XMVECTOR _target);

	//�X�V�i�r���[�s��쐬�j
	void Update();

	//���_�i�J�����̈ʒu�j��ݒ�
	void SetPosition(XMVECTOR _position);

	//�œ_�i����ʒu�j��ݒ�
	void SetTarget(XMVECTOR _target);

	void SetPlayerPointer(Player* _player);

	//�r���[�s����擾
	XMMATRIX GetViewMatrix();

	//�v���W�F�N�V�����s����擾
	XMMATRIX GetProjectionMatrix();

	//�ʒu���擾
	XMVECTOR GetPosition();

	//�œ_���擾
	XMVECTOR GetTarget();
};