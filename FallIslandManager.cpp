#include "FallIslandManager.h"
#include"Engine/DeltaTime.h"
#include"Engine\Sprite.h"
#include"Engine\JsonReader.h"
#include"FallBlockManager.h"
#include"GameSetting.h"


namespace GS = GameSetting;
namespace DT = DeltaTime;
namespace JR = JsonReader;

namespace FallIslandManagerSet {
	float FALL_SPEED(0);
	XMFLOAT3 FRAME_SIZE(0, 0., 0);
	XMFLOAT3 FRAME_POSITION(0, 0, 0);
	XMFLOAT3 ISLAND_SIZE(0, 0, 0);
	XMFLOAT3 ISLAND_POSITION(0, 0, 0);
	void Initialize(std::string _name) {
		JR::Get<float>(_name, "FALL_SPEED", FALL_SPEED);
		JR::Get<float>(_name, "FRAME_IMAGE_SIZE_X", FRAME_SIZE.x);
		JR::Get<float>(_name, "FRAME_IMAGE_SIZE_Y", FRAME_SIZE.y);
		JR::Get<float>(_name, "FRAME_IMAGE_POSITION_X", FRAME_POSITION.x);
		JR::Get<float>(_name, "FRAME_IMAGE_POSITION_Y", FRAME_POSITION.y);
		JR::Get<float>(_name, "ISLAND_IMAGE_SIZE_X", ISLAND_SIZE.x);
		JR::Get<float>(_name, "ISLAND_IMAGE_SIZE_Y", ISLAND_SIZE.y);
		JR::Get<float>(_name, "ISLAND_IMAGE_POSITION_X", ISLAND_POSITION.x);
		JR::Get<float>(_name, "ISLAND_IMAGE_POSITION_Y", ISLAND_POSITION.y);
	}
}

namespace Set = FallIslandManagerSet;

FallIslandManager::FallIslandManager(GameObject* parent)
	: GameObject(parent, "FallIslandManager"),frame_(nullptr),island_(nullptr),
	fbManager_(nullptr),percentage_(1),tmpPercentage_(1)
{
}

FallIslandManager::~FallIslandManager()
{
}

void FallIslandManager::Initialize()
{
	Set::Initialize(objectName_);
	frame_ = new Sprite();
	frame_->Load(JR::Get<std::string>(objectName_,"IMAGE_PATH_FRAME"));
	island_ = new Sprite();
	island_->Load(JR::Get<std::string>(objectName_, "IMAGE_PATH_GAUGE"));

	transform_.scale_ = Set::FRAME_SIZE;
	transform_.position_ = Set::FRAME_POSITION;
	islandTrans_.scale_ = Set::ISLAND_SIZE;
	islandTrans_.position_ = Set::ISLAND_POSITION;
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
		percentage_ -= Set::FALL_SPEED * DT::GetDeltaTime();
		if (tmpPercentage_ > percentage_)
			percentage_ = tmpPercentage_;
	}
	else if (tmpPercentage_ > percentage_) {
		percentage_ += Set::FALL_SPEED * DT::GetDeltaTime();
		if (tmpPercentage_ < percentage_)
			percentage_ = tmpPercentage_;
	}
	islandTrans_.position_.y = Set::ISLAND_POSITION.y * percentage_;
}
