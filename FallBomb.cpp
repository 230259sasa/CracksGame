#include "FallBomb.h"
#include "Engine\Model.h"
#include "Engine\JsonReader.h"
#include "Stage.h"
#include "Timer.h"

namespace JR = JsonReader;

FallBomb::FallBomb(GameObject* _parent) 
	:FallObject(_parent, "FallBomb"),explosionTimer_(nullptr),
	explosion_time_(JR::Get<float>(objectName_, "explosion_time"))
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
	
	if (explosionTimer_->IsTimeOver(explosion_time_)) {
		Stage* stage = (Stage*)FindObject("Stage");
		if (stage == nullptr)
			return;
		explosionTimer_->ResetTimer();
		XMINT3 pos = {	(int)transform_.position_.x,
						(int)transform_.position_.y,
						(int)transform_.position_.z };
		int size = 3;
		int center = size / 2;
		int end = size - center;
		int start = -center;
		for (int x = start;x < end;x++) {
			for (int y = start;y < end;y++) {
				for (int z = start;z < end;z++) {
					stage->BreakBlock(pos.x + x, pos.y + y, pos.z + z);
				}
			}
		}
	}
}
