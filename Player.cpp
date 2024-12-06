#include "Player.h"
#include"Engine\Model.h"
#include"Engine\Input.h"
#include"Engine\Camera.h"
#include"Engine\DeltaTime.h"
#include"Stage.h"
#include<numbers>

namespace Set {
	const float MOVE_SPEED(0.1f);
	const int LEFT_MOVE_ANGLE(90);
	const int RIGHT_MOVE_ANGLE(270);

	const float GRAVITY(25.0f);
	const float JUMP_HEIGHT(1);
	const float JUMP_LAUNCH_SPEED(sqrtf(2 * GRAVITY * JUMP_HEIGHT));
	const float MAX_FALL_VELOCITY(-10.0f);
	const float LANDING_DISTANCE(0.1f);
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
	hModel_ = Model::Load("Assets/Model/BoxBrick.fbx");
	assert(hModel_ >= 0);
	//Stageのブロックに重ならないために足している
	transform_.position_.y += 1;

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
	//デバック用の移動

	//移動キーが押されてなければreturn
	/*if (!Input::IsKey(DIK_W) && !Input::IsKey(DIK_S) &&
		!Input::IsKey(DIK_A) && !Input::IsKey(DIK_D))
		return;*/
	
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
	else if (Input::IsKey(DIK_S)) {
		vectorX *= -1;
		vectorZ *= -1;
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

	//移動
	transform_.position_.x += Set::MOVE_SPEED * vectorX;
	transform_.position_.z += Set::MOVE_SPEED * vectorZ;
}

void Player::Jump()
{
	if (Input::IsKeyDown(DIK_SPACE) && isGround_) {
		isGround_ = false;
		jumpVelocity_ = Set::JUMP_LAUNCH_SPEED;
		transform_.position_.y += Set::LANDING_DISTANCE;
	}
}

void Player::Fall()
{
	//ステージ上のブロックとレイキャスト
	float dist = 0.0f;
	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage != nullptr) {
		RayCastData rayData;
		XMFLOAT3 pos = transform_.position_;
		rayData.start = { pos.x,pos.y,pos.z,0.0f };
		rayData.dir = { 0,-1,0,0 };
		rayData.hit = false;
		stage->StageBlockRayCast(rayData);
		dist = rayData.dist - (stage->GetBlockSize().x / 2);
		if (rayData.hit && dist < Set::LANDING_DISTANCE) {
			transform_.position_.y = (transform_.position_.y - rayData.dist + stage->GetBlockSize().x / 2);
			isGround_ = true;
			jumpVelocity_ = 0.0f;
		}
		else {
			isGround_ = false;
		}
	}

	if (!isGround_) {
		jumpVelocity_ -= Set::GRAVITY * DT::GetDeltaTime();
	}

	if (jumpVelocity_ < Set::MAX_FALL_VELOCITY) {
		jumpVelocity_ = Set::MAX_FALL_VELOCITY;
	}

	transform_.position_.y += jumpVelocity_ * DT::GetDeltaTime();
}
