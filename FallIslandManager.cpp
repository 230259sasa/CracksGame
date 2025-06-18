#include "FallIslandManager.h"
#include"Engine/DeltaTime.h"
#include"Engine\Sprite.h"
#include"FallBlockManager.h"
#include"GameSetting.h"

namespace GS = GameSetting;
namespace DT = DeltaTime;

namespace Set {
	const float FALL_SPEED(0.1);
	const XMFLOAT3 FRAME_SIZE(0.2, 0.5, 0);
	const XMFLOAT3 FRAME_POSITION(-0.78, 0.445, 0);
	const XMFLOAT3 ISLAND_SIZE(0.08, 0.05, 0);
	const XMFLOAT3 ISLAND_POSITION(-0.78, 0.85, 0);
}

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
	frame_ = new Sprite();
	frame_->Load("Frame.png");
	island_ = new Sprite();
	island_->Load("Gauge.png");

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
