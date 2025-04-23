#include "PlaySceneReady.h"
#include"Engine/DeltaTime.h"

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
}

void PlaySceneReady::Release()
{
}

void PlaySceneReady::Update()
{
}

void PlaySceneReady::Draw()
{
}

bool PlaySceneReady::IsReadyFinish()
{
	return isReadyFinish_;
}
