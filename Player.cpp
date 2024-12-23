#include "Player.h"
#include"Engine\Model.h"
#include"Engine\Input.h"
#include"Engine\Camera.h"
#include"Engine\DeltaTime.h"
#include"Stage.h"
#include<numbers>

namespace Set {
	const float PLAYER_RADIUS(0.5f);
	const float MOVE_SPEED(0.1f);
	const int LEFT_MOVE_ANGLE(90);
	const int RIGHT_MOVE_ANGLE(270);

	const float GRAVITY(25.0f);
	const float JUMP_HEIGHT(1.5);//�W�����v�̍���
	const float JUMP_LAUNCH_SPEED(sqrtf(2 * GRAVITY * JUMP_HEIGHT));//�W�����v�̏���
	const float MAX_FALL_VELOCITY(-10.0f);//�����̍ő呬�x
	const float FALL_CORRECTION_Y(0.01f);//��������RayCast�̊J�n���WY�ɑ����␳�l
}

namespace DT = DeltaTime;

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hModel_(-1), jumpVelocity_(0.0f), isGround_(true)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Assets/Model/TestPlayer.fbx");
	assert(hModel_ >= 0);
	//Stage�̃u���b�N�ɏd�Ȃ�Ȃ����߂ɑ����Ă���
	transform_.position_.y += 2;

	Camera::SetPlayerPointer(this);
}

void Player::Release()
{
}

void Player::Update()
{
	Move();
	Jump();
	Fall();
}

void Player::Draw()
{
	Transform trans = transform_;
	trans.position_.x = XMVectorGetX(Camera::GetTarget());
	trans.position_.z = XMVectorGetZ(Camera::GetTarget());
	Model::SetTransform(hModel_, trans);
	Model::Draw(hModel_);
}

void Player::Move()
{
	//�f�o�b�N�p�̈ړ�

	//�ړ��L�[��������ĂȂ����return
	/*if (!Input::IsKey(DIK_W) && !Input::IsKey(DIK_S) &&
		!Input::IsKey(DIK_A) && !Input::IsKey(DIK_D))
		return;*/
	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;
	
	//�x�N�g�������߂�
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

	//�ړ�������������
	if (Input::IsKey(DIK_W)) {
	}
	else if (Input::IsKey(DIK_S)) {
		vectorX = -vectorX;
		vectorZ = -vectorZ;
	}
	else if (Input::IsKey(DIK_A)) {
		constexpr float r = XMConvertToRadians(Set::LEFT_MOVE_ANGLE);
		float x = vectorX * cos(r) - vectorZ * sin(r);
		float z = vectorX * sin(r) + vectorZ * cos(r);
		vectorX = x;
		vectorZ = z; 
	}
	else if (Input::IsKey(DIK_D)) {
		constexpr float r = XMConvertToRadians(Set::RIGHT_MOVE_ANGLE);
		float x = vectorX * cos(r) - vectorZ * sin(r);
		float z = vectorX * sin(r) + vectorZ * cos(r);
		vectorX = x;
		vectorZ = z;
	}
	else {
		vectorX = 0;
		vectorZ = 0;
	}


	if (Input::IsKey(DIK_UP)) {
		transform_.position_.y += 1.0f;
		vectorX = 0;
		vectorZ = 0;
	}


	XMFLOAT3 dir(0, 0, 0);
	XMFLOAT3 move(0, 0, 0), push(0, 0, 0);
	dir.x = vectorX;
	dir.z = vectorZ;
	if (dir.x != 0 || dir.z != 0) {
		XMFLOAT3 pos = transform_.position_;
		//model�̒��S�ʒu�Ŕ��肷�邽�߂ɑ����Ă���
		//���ꂪ�Ȃ��ƒn�ʂɓ������ĉ����Ԃ��Ă��܂�
		pos.y += 0.6;
		move.x = Set::MOVE_SPEED * vectorX;
		move.z = Set::MOVE_SPEED * vectorZ;
		pos.x += move.x;
		pos.z += move.z;
		push = stage->GetPushBack(pos, Set::PLAYER_RADIUS);
		move.x += push.x;
		move.z += push.z;
	}
	//�ړ�
	transform_.position_.x += move.x;
	transform_.position_.z += move.z;
}

void Player::Jump()
{
	if (Input::IsKeyDown(DIK_SPACE) &&  isGround_) {
		isGround_ = false;
		jumpVelocity_ = Set::JUMP_LAUNCH_SPEED;
	}
}

void Player::Fall()
{
	if (!isGround_) {
		jumpVelocity_ -= Set::GRAVITY * DT::GetDeltaTime();
	}

	if (jumpVelocity_ < Set::MAX_FALL_VELOCITY) {
		jumpVelocity_ = Set::MAX_FALL_VELOCITY;
	}

	float fallSpeed = jumpVelocity_ * DT::GetDeltaTime();
	transform_.position_.y += fallSpeed;

	//�X�e�[�W��̃u���b�N�ƃ��C�L���X�g
	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage != nullptr) {
		RayCastData rayData;
		XMFLOAT3 pos = transform_.position_;
		rayData.start = { pos.x,pos.y + Set::FALL_CORRECTION_Y,pos.z,0.0f };
		rayData.dir = { 0,-1,0,0 };
		rayData.hit = false;
		stage->StageBlockRayCast(rayData);
		float dist = 0.0f;
 		dist = rayData.dist;
		//���ݎg�p���Ă���model�̒�ӂ�-0.5�Ȃ̂œ���Ă���
		//model�ύX���ɍ폜
		//dist -= 0.5f;

		if (rayData.hit && dist <= abs(fallSpeed) && jumpVelocity_ <= 0) {
			transform_.position_.y -= dist;
			isGround_ = true;
			jumpVelocity_ = 0.0f;
		}
		else {
			isGround_ = false;
		}
	}
}
