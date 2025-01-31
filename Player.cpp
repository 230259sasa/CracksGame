#include "Player.h"
#include"Engine\Model.h"
#include"Engine\Input.h"
#include"Engine\Camera.h"
#include"Engine\DeltaTime.h"
#include"Stage.h"
#include "FallBlockManager.h"
#include<numbers>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace Set {
	const float GRAVITY(25.0f);
	const float MAX_FALL_VELOCITY(-10.0f);//落下の最大速度
	//const float FALL_CORRECTION_Y(0.1f);//落下時のRayCastの開始座標Yに足す補正値
	const XMFLOAT3 FORWARD_VECTOR(0, 0, 1);//前方へのベクトル

	const int LEFT_MOVE_ANGLE(90);
	const int RIGHT_MOVE_ANGLE(270);
	const float PLAYER_RADIUS(0.4f);
	const float MOVE_SPEED(0.05f);
	const float JUMP_HEIGHT(1.5);//ジャンプの高さ
	const float JUMP_LAUNCH_SPEED(sqrtf(2 * GRAVITY * JUMP_HEIGHT));//ジャンプの初速

	const int CAMERA_ROTATE_SPEED(5);
}

namespace DT = DeltaTime;

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hModel_(-1), jumpVelocity_(0.0f), isGround_(true),
	framePos_({ 0,0,0 }), pastPos_(0, 0, 0)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Assets/Model/TestPlayer2.fbx");
	assert(hModel_ >= 0);
	//Stageのブロックに重ならないために足している
	transform_.position_.y += 2.0f;
	framePos_ = { (int)transform_.position_.x, (int)transform_.position_.y-1,
		(int)transform_.position_.z+1 };
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
	Relocate();
	MoveCamera();
	BreakStageBlock();
}

void Player::Draw()
{
	/*{
		ImGui::Text("position x=%5.3li,y=%5.3li,z=%5.3li", (int)transform_.position_.x,
			(int)transform_.position_.y, (int)transform_.position_.z);
		ImGui::Text("position x=%5.3lf,y=%5.3lf,z=%5.3lf", transform_.position_.x,
			transform_.position_.y, transform_.position_.z);

	}*/
	Transform trans = transform_;
	trans.position_.x = XMVectorGetX(Camera::GetTarget());
	trans.position_.z = XMVectorGetZ(Camera::GetTarget());
	trans.scale_ = XMFLOAT3(0.8, 1, 0.8);
	Model::SetTransform(hModel_, trans);
	Model::Draw(hModel_);
	
	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;
	stage->DrawFrame({(float)framePos_.x,(float)framePos_.y,(float)framePos_.z});
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
		move.x = Set::MOVE_SPEED * vectorX;
		move.z = Set::MOVE_SPEED * vectorZ;
		pos.x += move.x;
		pos.y += 0.1f;
		pos.z += move.z;
		push = stage->GetPushBack(pos, Set::PLAYER_RADIUS);
		move.x += push.x;
		move.z += push.z;

		//移動方向をプレイヤーが向く
		transform_.rotate_.y = XMConvertToDegrees(
			atan2(Set::FORWARD_VECTOR.z, Set::FORWARD_VECTOR.x) -
			atan2(vectorZ, vectorX));
	}
	//移動
	pastPos_ = { (int)transform_.position_.x,(int)transform_.position_.y,
		(int)transform_.position_.z };
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

	//ステージ上のブロックとレイキャスト
	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage != nullptr) {
		for (int i = 0; i < 8; i++) {
			RayCastData rayData;
			XMFLOAT3 pos = transform_.position_;
			float r = XMConvertToRadians(45*i);
			float x = 0 * cos(r) - Set::PLAYER_RADIUS * sin(r);
			float z = 0 * sin(r) + Set::PLAYER_RADIUS * cos(r);
			rayData.start = { pos.x + x,pos.y,pos.z + z,0.0f };
			rayData.dir = { 0,-1,0,0 };
			rayData.hit = false;
			rayData.dist = 0;
			stage->FallRayCast(rayData);
			float dist = 0.0f;
			dist = rayData.dist;

			//レイキャストが当たったかつ距離が現在のフレームの落下距離より小さいかつ落下中ならtrue
			if (rayData.hit && dist <= abs(fallSpeed) && jumpVelocity_ <= 0) {
				transform_.position_.y -= dist;
				isGround_ = true;
				jumpVelocity_ = 0.0f;
				return;
			}
		}
	}
	isGround_ = false;
	transform_.position_.y += fallSpeed;
}

void Player::Relocate()
{
	if (transform_.position_.y < -5) {
		Stage* stage = nullptr;
		stage = (Stage*)FindObject("Stage");
		if (stage == nullptr)
			return;
		transform_.position_ = stage->GetScaffoldPos();
		transform_.position_.y += 1;
	}
}

void Player::MoveCamera()
{
	if (Input::IsKey(DIK_Q))
		Camera::RotateCameraLeft(Set::CAMERA_ROTATE_SPEED);
	else if (Input::IsKey(DIK_E))
		Camera::RotateCameraRight(Set::CAMERA_ROTATE_SPEED);
}

void Player::SetBlock()
{
	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;

	int x, y, z;
	x = (int)(transform_.position_.x);
	y = (int)(transform_.position_.y);
	z = (int)(transform_.position_.z);

	/*int dir = ((int)transform_.rotate_.y % 360) / 45;

	if (dir <= 0 || dir == 7) {
		z = (int)(transform_.position_.z) + 2;
	}
	else if (dir <= 2) {
		x = (int)(transform_.position_.x) + 2;
	}
	else if (dir <= 4) {
		z = (int)(transform_.position_.z) - 1;
	}
	else if (dir <= 6) {
		x = (int)(transform_.position_.x) - 1;
	}*/

	float dir = ((int)transform_.rotate_.y % 360) / (360/16);

	if (dir <= 0 || dir == 15) {
		z = (int)(transform_.position_.z) + 2;
	}
	else if (dir <= 2) {
		z = (int)(transform_.position_.z) + 2;
		x = (int)(transform_.position_.x) + 2;
	}
	else if (dir <= 4) {
		x = (int)(transform_.position_.x) + 2;
	}
	else if (dir <= 6) {
		x = (int)(transform_.position_.x) + 2;
		z = (int)(transform_.position_.z) - 1;
	}
	else if (dir <= 8) {
		z = (int)(transform_.position_.z) - 1;
	}
	else if (dir < 10) {
		z = (int)(transform_.position_.z) - 1;
		x = (int)(transform_.position_.x) - 1;
	}
	else if (dir <= 12) {
		x = (int)(transform_.position_.x) - 1;
	}
	else if(dir <= 14) {
		x = (int)(transform_.position_.x) - 1;
		z = (int)(transform_.position_.z) + 2;
	}

	if (Input::IsKey(DIK_F)) {
		stage->SetBlock(x, y, z);
	}
}

void Player::BreakStageBlock()
{
	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;

	int x, y, z;
	x = (int)(transform_.position_.x);
	y = (int)(transform_.position_.y) - 1;
	z = (int)(transform_.position_.z);

	int dir = ((int)transform_.rotate_.y % 360) / 45;

	//前
	if (dir <= 0 || dir == 7) {
		z = (int)(transform_.position_.z) + 1;
	}
	//右
	else if (dir <= 2) {
		x = (int)(transform_.position_.x) + 1;
	}
	//後ろ
	else if (dir <= 4) {
		z = (int)(transform_.position_.z) - 1;
	}
	//左
	else if (dir <= 6) {
		x = (int)(transform_.position_.x) - 1;
	}

	if (Input::IsKeyDown(DIK_J)) {
		for(int sy = stage->GetStageSize().y; sy>=0;sy--){
			stage->SetNoneBlock(x, sy, z);
		}
	}

	if (pastPos_.x != transform_.position_.x || pastPos_.z != transform_.position_.z) {
		int height = 0;
		for (int sy = stage->GetStageSize().y; sy >= 0; sy--) {
			if (stage->GetIsOnBlock({ x,sy,z }) && height == 0)
				height = sy;
		}
		framePos_ = XMINT3(x, height, z);
	}
}
