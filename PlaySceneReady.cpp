#include "PlaySceneReady.h"
#include"Engine/DeltaTime.h"
#include"Engine/Sprite.h"
#include"Engine\JsonReader.h"
#include"Timer.h"
#include"FallBlockManager.h"
#include<sstream>

namespace DT = DeltaTime;
namespace JR = JsonReader;

//namespace Set {
//	const float PLAY_SCENE_READY_TIME(5.0f);
//	const float START_IMAGE_POSITION_Y(0.5f);
//	const float IMAGE_SIZE_X(0.1f);
//	const float IMAGE_SIZE_Y(0.1f);
//}

PlaySceneReady::PlaySceneReady(GameObject* parent)
	:GameObject(parent, "PlaySceneReady"),count_(MAX-1),
	play_scene_ready_time_(JR::Get<float>(objectName_,"ready_time")),
	start_image_position_y_(JR::Get<float>(objectName_, "start_image_position_y"))
{
}

PlaySceneReady::~PlaySceneReady()
{
}

void PlaySceneReady::Initialize()
{
	timer_ = new Timer();

	tex_ = new Sprite();
	//
	std::stringstream str;
	str << count_;
	tex_->Load(objectName_ + "/" + str.str() + ".png");

	Stop();

	////debug
	transform_.scale_ = { JR::Get<float>(objectName_,"image_size_x"), JR::Get<float>(objectName_,"image_size_y"),0};
}

void PlaySceneReady::Release()
{
}

void PlaySceneReady::Update()
{
	UpDownImage(transform_);
}

void PlaySceneReady::Draw()
{
	if (tex_ != nullptr)
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
	if (timer_->IsTimeOver(play_scene_ready_time_ / COUNT::MAX) && count_ <= 0) {
		timer_->StopTimer();
		Start();
		tex_ = nullptr;
	}
	else if (timer_->IsTimeOver(play_scene_ready_time_ / COUNT::MAX)) {
		timer_->ResetTimer();
		count_--;
		std::stringstream str;
		str << count_;
		tex_->Load(objectName_ + "/" + str.str() + ".png");
	}
	else {
		float seconds = timer_->GetSeconds();
		float d = seconds / ((play_scene_ready_time_ / COUNT::MAX));
		float y = start_image_position_y_ * d;
		_trans.position_.y = start_image_position_y_ - y;
	}
}
