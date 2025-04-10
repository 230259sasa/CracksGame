#include "FallIslandManager.h"
#include "Engine\Sprite.h"

FallIslandManager::FallIslandManager(GameObject* parent)
	: GameObject(parent, "FallIslandManager"),frame_(nullptr),island_(nullptr)
{
}

FallIslandManager::~FallIslandManager()
{
}

void FallIslandManager::Initialize()
{
	frame_ = new Sprite();
	frame_->Load("Assets\\Image\\Frame.png");
	island_ = new Sprite();
	island_->Load("Assets\\Image\\Gauge.png");

	transform_.scale_ = { 0.2,0.5,0 };
	transform_.position_ = { -0.78,0.48,0 };
	islandTrans_.scale_ = { 0.08,0.05,0 };
	islandTrans_.position_ = { -0.78,0.48,0 };
}

void FallIslandManager::Release()
{
}

void FallIslandManager::Update()
{
}

void FallIslandManager::Draw()
{
	island_->Draw(islandTrans_);
	frame_->Draw(transform_);
}
