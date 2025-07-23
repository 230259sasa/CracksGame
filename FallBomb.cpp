#include "FallBomb.h"
#include "Engine\Model.h"
#include "Engine\JsonReader.h"
#include "Stage.h"
#include "Timer.h"

namespace JR = JsonReader;

FallBomb::FallBomb(GameObject* _parent) 
	:FallObject(_parent, "FallBomb"),explosionTimer_(nullptr)
{
}

FallBomb::~FallBomb()
{
}

void FallBomb::Initialize()
{
	hModel_ = Model::Load(JR::Get<std::string>(objectName_, "model_path"));

	explosionTimer_ = new Timer();
}

void FallBomb::Release()
{
}

void FallBomb::Update()
{
	Fall();
	Explosion();
	Dead();
}

void FallBomb::Explosion()
{
	if (!IsOnGround()) {
		explosionTimer_->StopTimer();
		return;
	}
	else {
		explosionTimer_->StartTimer();
	}
	
	if (explosionTimer_->IsTimeOver(JR::Get<float>(objectName_, "explosion_time"))) {
		Stage* stage = (Stage*)FindObject("Stage");
		if (stage == nullptr)
			return;
		explosionTimer_->ResetTimer();
		XMINT3 pos = {	(int)transform_.position_.x,
						(int)transform_.position_.y,
						(int)transform_.position_.z };
		for (int x = -1;x < 2;x++) {
			for (int y = -1;y < 2;y++) {
				for (int z = -1;z < 2;z++) {
					stage->SetFallBlock(pos.x + x, pos.y + y, pos.z + z);
				}
			}
		}
	}
}
