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
	const float JUMP_HEIGHT(1.5);//ジャンプの高さ
	const float JUMP_LAUNCH_SPEED(sqrtf(2 * GRAVITY * JUMP_HEIGHT));//ジャンプの初速
	const float MAX_FALL_VELOCITY(-10.0f);//落下の最大速度
	const float FALL_CORRECTION_Y(0.01f);//落下時のRayCastの開始座標Yに足す補正値
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
	//Stageのブロックに重ならないために足している
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
	//デバック用の移動

	//移動キーが押されてなければreturn
	/*if (!Input::IsKey(DIK_W) && !Input::IsKey(DIK_S) &&
		!Input::IsKey(DIK_A) && !Input::IsKey(DIK_D))
		return;*/
	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
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
		//modelの中心位置で判定するために足している
		//これがないと地面に当たって押し返してしまう
		pos.y += 0.6;
		move.x = Set::MOVE_SPEED * vectorX;
		move.z = Set::MOVE_SPEED * vectorZ;
		pos.x += move.x;
		pos.z += move.z;
		push = stage->GetPushBack(pos, Set::PLAYER_RADIUS, dir);
		move.x += push.x;
		move.z += push.z;
	}
	//移動
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

	//ステージ上のブロックとレイキャスト
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
		//現在使用しているmodelの底辺が-0.5なので入れている
		//model変更時に削除
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
