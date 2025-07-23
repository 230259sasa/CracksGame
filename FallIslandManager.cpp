#include "FallIslandManager.h"
#include"Engine/DeltaTime.h"
#include"Engine\Sprite.h"
#include"Engine\JsonReader.h"
#include"FallBlockManager.h"
#include"GameSetting.h"


namespace GS = GameSetting;
namespace DT = DeltaTime;
namespace JR = JsonReader;

//namespace FallIslandManagerSet {
//	float FALL_SPEED(0);
//	XMFLOAT3 FRAME_SIZE(0, 0., 0);
//	XMFLOAT3 FRAME_POSITION(0, 0, 0);
//	XMFLOAT3 ISLAND_SIZE(0, 0, 0);
//	XMFLOAT3 ISLAND_POSITION(0, 0, 0);
//	void Initialize(std::string _name) {
//		JR::Get<float>(_name, "fall_speed", FALL_SPEED);
//		JR::Get<float>(_name, "frame_image_size_x", FRAME_SIZE.x);
//		JR::Get<float>(_name, "frame_image_size_y", FRAME_SIZE.y);
//		JR::Get<float>(_name, "frame_image_position_x", FRAME_POSITION.x);
//		JR::Get<float>(_name, "frame_image_position_y", FRAME_POSITION.y);
//		JR::Get<float>(_name, "island_image_size_x", ISLAND_SIZE.x);
//		JR::Get<float>(_name, "island_image_size_y", ISLAND_SIZE.y);
//		JR::Get<float>(_name, "island_image_position_x", ISLAND_POSITION.x);
//		JR::Get<float>(_name, "island_image_position_y", ISLAND_POSITION.y);
//	}
//}
//
//namespace Set = FallIslandManagerSet;

FallIslandManager::FallIslandManager(GameObject* parent)
	: GameObject(parent, "FallIslandManager"),frame_(nullptr),island_(nullptr),
	fbManager_(nullptr),percentage_(1),tmpPercentage_(1),
	fall_speed_(JR::Get<float>(objectName_, "fall_speed")),
	frame_size_({ JR::Get<float>(objectName_, "frame_image_size_x"),JR::Get<float>(objectName_, "frame_image_size_y"),0 }),
	frame_positoin_({ JR::Get<float>(objectName_, "frame_image_position_x"),JR::Get<float>(objectName_, "frame_image_position_y"),0 }),
	island_size_({ JR::Get<float>(objectName_, "island_image_size_x"),JR::Get<float>(objectName_, "island_image_size_y"),0 }),
	island_position_({ JR::Get<float>(objectName_, "island_image_position_x"),JR::Get<float>(objectName_, "island_image_position_y"),0 })
{
}

FallIslandManager::~FallIslandManager()
{
}

void FallIslandManager::Initialize()
{
	//Set::Initialize(objectName_);
	frame_ = new Sprite();
	frame_->Load(JR::Get<std::string>(objectName_,"image_path_frame"));
	island_ = new Sprite();
	island_->Load(JR::Get<std::string>(objectName_, "image_path_gauge"));

	transform_.scale_ = frame_size_;
	transform_.position_ = frame_positoin_;
	islandTrans_.scale_ = island_size_;
	islandTrans_.position_ = island_position_;
}

void FallIslandManager::Release()
{
}

void FallIslandManager::Update()
{
	RatioCalculation();
	Fall();
}

void FallIslandManager::Draw()
{
	island_->Draw(islandTrans_);
	frame_->Draw(transform_);
}

void FallIslandManager::RatioCalculation()
{
	if (fbManager_ == nullptr) {
		fbManager_ = (FallBlockManager*)FindObject("FallBlockManager");
	}
	else {
		tmpPercentage_ = (float)fbManager_->GetOnGroundBlockNum() / (float)GS::GetGameOverOnGroundBlockNum();
		tmpPercentage_ = 1 - tmpPercentage_;
	}
}

void FallIslandManager::Fall()
{
	if (tmpPercentage_ < percentage_) {
		percentage_ -= fall_speed_ * DT::GetDeltaTime();
		if (tmpPercentage_ > percentage_)
			percentage_ = tmpPercentage_;
	}
	else if (tmpPercentage_ > percentage_) {
		percentage_ += fall_speed_ * DT::GetDeltaTime();
		if (tmpPercentage_ < percentage_)
			percentage_ = tmpPercentage_;
	}
	islandTrans_.position_.y = island_position_.y * percentage_;
}
