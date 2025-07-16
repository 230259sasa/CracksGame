#include "PlaySceneReady.h"
#include"Engine/DeltaTime.h"
#include"Timer.h"
#include"Player.h"
#include"FallBlockManager.h"

namespace DT = DeltaTime;

namespace Set {
	const float PLAY_SCENE_READY_TIME(5.0f);
}

PlaySceneReady::PlaySceneReady(GameObject* parent)
	:GameObject(parent, "PlaySceneReady")
{
}

PlaySceneReady::~PlaySceneReady()
{
}

void PlaySceneReady::Initialize()
{
	timer_ = new Timer();
}

void PlaySceneReady::Release()
{
}

void PlaySceneReady::Update()
{
	Stop();
	if (timer_->IsTimeOver(Set::PLAY_SCENE_READY_TIME)) {
		Start();
	}
}

void PlaySceneReady::Draw()
{
}

void PlaySceneReady::Stop()
{
	FallBlockManager* fbm = (FallBlockManager*)FindObject("FallBlockManager");
	if (fbm != nullptr) {
		fbm->StopSpawnBlock();
	}
	Player* player = (Player*)FindObject("Player");
	if (player != nullptr) {
		//player->StopAnimation();
	}

	DT::Stop();
}

void PlaySceneReady::Start()
{
	FallBlockManager* fbm = (FallBlockManager*)FindObject("FallBlockManager");
	if (fbm != nullptr) {
		fbm->StartSpawnBlock();
	}
	Player* player = (Player*)FindObject("Player");
	if (player != nullptr) {
		//player->Initialize();
	}

	DT::Run();
}
