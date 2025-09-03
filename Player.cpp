#include "Player.h"
#include"Engine\Model.h"
#include"Engine\Input.h"
#include"Engine\Camera.h"
#include"Engine\DeltaTime.h"
#include"Engine\JsonReader.h"
#include"Stage.h"
#include "FallBlockManager.h"
#include"PlayerAnimContext.h"
#include<numbers>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace DT = DeltaTime;
namespace JR = JsonReader;

namespace PlayerSet {
	const float GRAVITY(25.0f);//別のに入れたい
	//const float FALL_CORRECTION_Y(0.1f);//落下時のRayCastの開始座標Yに足す補正値
	const XMFLOAT3 FORWARD_VECTOR(0, 0, 1);//前方へのベクトル

	const int DEGREES_360(360);
	const int LEFT_MOVE_ANGLE(90);
	const int BACK_MOVE_ANGLE(180);
	const int RIGHT_MOVE_ANGLE(270);
	const int NUM_SECTORS(8);
	const int SECTOR_ANGLE(DEGREES_360 / NUM_SECTORS);

	const int CAMERA_ROTATE_SPEED(3);
	const int CAMERA_ROTATE_ANGLE(90);
}

namespace Set = PlayerSet;

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), jumpVelocity_(0.0f), isGround_(true),
	framePos_({ 0,0,0 }), pastPos_(0, 0, 0), isCameraRotateStart_(false),
	CameraRotateDir_(0), heldObject_(nullptr), animContext_(new PlayerAnimContext(this)),
	animID_(AT::STAND),
	move_speed_(JR::Get<float>(objectName_, "move_speed")),
	player_radius_(JR::Get<float>(objectName_, "player_radius")),
	initial_jump_velocity_(sqrtf(JR::Get<float>(objectName_, "jump_height")*Set::GRAVITY*2)),
	max_fall_velocity_(JR::Get<float>(objectName_, "max_fall_velocity"))
{
	for (int i = 0;i < FUNCTION_INDEX_MAX;i++) {
		isFunctionEnabled_[0] = true;
	}
	for (int i = 0;i < ANIM_TYPE_MAX;i++) {
		animData_[i].animFrameNum = 0;
		animData_[i].hModel = 0;
		animData_[i].isAnimAction = false;
	}
}

Player::~Player()
{
}

void Player::Initialize()
{
	//hModel_ = Model::Load("Player/.fbx");
	//assert(hModel_ >= 0);
	animData_[(int)AT::STAND].hModel = Model::Load(JR::Get<std::string>(objectName_,"model_path_stand"));
	int frameNum = 0;
	JR::Get<int>(objectName_, "frame_num_stand",frameNum);
	Model::SetAnimFrame(animData_[(int)AT::STAND].hModel, 0, frameNum);
	animData_[(int)AT::STAND].animFrameNum = frameNum;
	animData_[(int)AT::PUNCH].hModel = Model::Load(JR::Get<std::string>(objectName_, "model_path_punch"));
	JR::Get<int>(objectName_, "frame_num_punch", frameNum);
	Model::SetAnimFrame(animData_[(int)AT::PUNCH].hModel, 0, frameNum);
	animData_[(int)AT::PUNCH].animFrameNum = frameNum;
	animData_[(int)AT::MOVE].hModel = Model::Load(JR::Get<std::string>(objectName_, "model_path_run"));
	JR::Get<int>(objectName_, "frame_num_run", frameNum);
	Model::SetAnimFrame(animData_[(int)AT::MOVE].hModel, 0, frameNum);
	animData_[(int)AT::MOVE].animFrameNum = frameNum;

	//Stageのブロックに重ならないために足している
	transform_.position_.y = 2.0f;
	framePos_ = { (int)transform_.position_.x, (int)transform_.position_.y - 1,
		(int)transform_.position_.z + 1 };
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

	AnimationManager();
}

void Player::Draw()
{
	{
		/*ImGui::Text("position x=%5.3li,y=%5.3li,z=%5.3li", (int)transform_.position_.x,
			(int)transform_.position_.y, (int)transform_.position_.z);
		ImGui::Text("position x=%5.3lf,y=%5.3lf,z=%5.3lf", transform_.position_.x,
			transform_.position_.y, transform_.position_.z);*/
			//ImGui::Text("animframe=%5.3li", Model::GetAnimFrame(animData_[animID_].hModel));
	}
	Transform trans = transform_;
	//trans.position_.x = XMVectorGetX(Camera::GetTarget());
	//trans.position_.z = XMVectorGetZ(Camera::GetTarget());
	trans.scale_ = XMFLOAT3(0.8, 1, 0.8);
	Model::SetTransform(animData_[(int)animID_].hModel, trans);
	Model::Draw(animData_[(int)animID_].hModel);

	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;
	stage->DrawFrame({ (float)framePos_.x,(float)framePos_.y,(float)framePos_.z });
}

bool Player::GetIsAnimAction(AnimType _type)
{
	return animData_[(int)_type].isAnimAction;
}

void Player::AnimationManager()
{
	AnimType id = animContext_->GetCurrentAnimType();
	if (id != animID_) {
		animID_ = id;
		animContext_->ChangeAnimType(animID_);
		std::array<bool, FUNCTION_INDEX_MAX> arr = animContext_->GetIsFunctionArr();
	}
}

void Player::Move()
{
	int angle = Camera::GetRotateAngle() % Set::DEGREES_360;

	//ラジアンに変換
	float rad = XMConvertToRadians(angle);

	// 正規化された方向ベクトル
	float vectorX = -std::sin(rad);
	float vectorZ = std::cos(rad);

	float r = XMConvertToRadians(0);
	//移動する方向を入力
	if (Input::IsKey(DIK_W)) {
	}
	else if (Input::IsKey(DIK_S)) {
		r = XMConvertToRadians(Set::BACK_MOVE_ANGLE);
	}
	else if (Input::IsKey(DIK_A)) {
		r = XMConvertToRadians(Set::LEFT_MOVE_ANGLE);
	}
	else if (Input::IsKey(DIK_D)) {
		r = XMConvertToRadians(Set::RIGHT_MOVE_ANGLE);
	}
	else {
		vectorX = 0;
		vectorZ = 0;
	}

	float x = vectorX * cos(r) - vectorZ * sin(r);
	float z = vectorX * sin(r) + vectorZ * cos(r);
	vectorX = x;
	vectorZ = z;

	XMFLOAT3 dir(0, 0, 0);
	XMFLOAT3 move(0, 0, 0), push(0, 0, 0);
	dir.x = vectorX;
	dir.z = vectorZ;

	animData_[(int)AT::MOVE].isAnimAction = false;
	if (dir.x != 0 || dir.z != 0) {
		animData_[(int)AT::MOVE].isAnimAction = true;
		Stage* stage = nullptr;
		stage = (Stage*)FindObject("Stage");
		if (stage == nullptr)
			return ;

		XMFLOAT3 pos = transform_.position_;
		float speed = move_speed_ * DT::GetDeltaTime();
		move.x = speed * vectorX;
		move.z = speed * vectorZ;
		pos.x += move.x;
		//pos.y += 0.1f;
		pos.z += move.z;
		push = stage->GetPushBack(pos, player_radius_);
		move.x += push.x;
		move.z += push.z;

		//移動方向をプレイヤーが向く
		transform_.rotate_.y = XMConvertToDegrees(
			atan2(Set::FORWARD_VECTOR.z, Set::FORWARD_VECTOR.x) -
			atan2(vectorZ, vectorX));
		//移動
		pastPos_ = { (int)transform_.position_.x,(int)transform_.position_.y,
			(int)transform_.position_.z };
		transform_.position_.x += move.x;
		transform_.position_.z += move.z;
	}
}

void Player::Jump()
{
	if (Input::IsKeyDown(DIK_SPACE) &&  isGround_) {
		isGround_ = false;
		jumpVelocity_ = initial_jump_velocity_;
	}
}

void Player::Fall()
{
	if (!isGround_) {
		jumpVelocity_ -= Set::GRAVITY * DT::GetDeltaTime();
	}

	if (jumpVelocity_ < max_fall_velocity_) {
		jumpVelocity_ = max_fall_velocity_;
	}

	float fallSpeed = jumpVelocity_ * DT::GetDeltaTime();

	//ステージ上のブロックとレイキャスト
	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage != nullptr) {
		float radius = player_radius_;
		for (int i = 0; i < Set::NUM_SECTORS; i++) {
			RayCastData rayData;
			XMFLOAT3 pos = transform_.position_;
			float r = XMConvertToRadians(Set::SECTOR_ANGLE*i);
			float x = 0 * cos(r) - radius * sin(r);
			float z = 0 * sin(r) + radius * cos(r);
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
	if (Input::IsKey(DIK_Q)) {
		CameraRotateDir_ = CaneraDir::LEFT;
		isCameraRotateStart_ = true;
	}
	else if (Input::IsKey(DIK_E)) {
		CameraRotateDir_ = CaneraDir::RIGHT;
		isCameraRotateStart_ = true;
	}

	if (isCameraRotateStart_) {
		Camera::RotateCamera(CameraRotateDir_ * Set::CAMERA_ROTATE_SPEED);
		if (Camera::GetRotateAngle()%Set::CAMERA_ROTATE_ANGLE == 0) {
			isCameraRotateStart_ = false;
		}
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
	y = (int)(transform_.position_.y);
	z = (int)(transform_.position_.z);

	int angle = ((int)transform_.rotate_.y % Set::DEGREES_360);
	if (angle < 0)
		angle = Set::DEGREES_360 + angle;
	int dir = angle / Set::SECTOR_ANGLE;
	
	int arrX[Set::NUM_SECTORS] = { 0,1,1,0,0,-1,-1,0 };
	int arrZ[Set::NUM_SECTORS] = { 1,0,0,-1,-1,0,0,1 };

	x = (int)(transform_.position_.x) + arrX[dir];
	z = (int)(transform_.position_.z) + arrZ[dir];

	if (Input::IsKeyDown(DIK_J)) {
		for(int sy = stage->GetStageSize().y; sy>=0;sy--){
			stage->SetFallBlock(x, sy, z);
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
