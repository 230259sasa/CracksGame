#include "PlaySceneReady.h"
#include"Engine/DeltaTime.h"
#include"Engine/Sprite.h"
#include"Timer.h"
#include"FallBlockManager.h"

namespace DT = DeltaTime;

namespace Set {
	const float PLAY_SCENE_READY_TIME(5.0f);
	const float START_IMAGE_POSITION_Y(0.5f);
	const float IMAGE_SIZE_X(0.1f);
	const float IMAGE_SIZE_Y(0.1f);
}

PlaySceneReady::PlaySceneReady(GameObject* parent)
	:GameObject(parent, "PlaySceneReady"),count_(MAX)
{
}

PlaySceneReady::~PlaySceneReady()
{
}

void PlaySceneReady::Initialize()
{
	timer_ = new Timer();

	tex_ = new Sprite();
	tex_->Load("TITLE.png");

	////debug
	transform_.scale_ = { Set::IMAGE_SIZE_X,Set::IMAGE_SIZE_Y,0 };
}

void PlaySceneReady::Release()
{
}

void PlaySceneReady::Update()
{
	Stop();
	UpDownImage(transform_);
}

void PlaySceneReady::Draw()
{
	tex_->Draw(transform_);
}

void PlaySceneReady::Stop()
{
	FallBlockManager* fbm = (FallBlockManager*)FindObject("FallBlockManager");
	if (fbm != nullptr) {
		fbm->StopSpawnBlock();
	}

	DT::Stop();
}

void PlaySceneReady::Start()
{
	FallBlockManager* fbm = (FallBlockManager*)FindObject("FallBlockManager");
	if (fbm != nullptr) {
		fbm->StartSpawnBlock();
	}

	DT::Run();
}

void PlaySceneReady::UpDownImage(Transform& _trans)
{
	if (timer_->IsTimeOver(Set::PLAY_SCENE_READY_TIME / COUNT::MAX)) {
		timer_->ResetTimer();
		count_--;
	}
	else {
		float seconds = timer_->GetSeconds();
		float d = seconds / ((Set::PLAY_SCENE_READY_TIME / COUNT::MAX));
		float y = Set::START_IMAGE_POSITION_Y * d;
		_trans.position_.y = Set::START_IMAGE_POSITION_Y - y;
	}
}
