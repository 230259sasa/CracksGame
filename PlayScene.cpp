#include "PlayScene.h"
#include"Engine/Input.h"
#include"Engine\SceneManager.h"
#include"Stage.h"
#include"Player.h"
#include"FallBlockManager.h"
#include"PlayStateCheck.h"
#include"PlaySceneBackground.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent, "PlayScene")
{
}

void PlayScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<FallBlockManager>(this);
	Instantiate<Player>(this);
	Instantiate<PlayStateCheck>(this);
	//Instantiate<PlaySceneBackground>(this);
}

void PlayScene::Update()
{
	PlayStateCheck* state = nullptr;
	state = (PlayStateCheck*)FindObject("PlayStateCheck");
	if (state == nullptr)
		return;
	if (state->IsClear()) {
		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM != nullptr)
		pSM->ChangeScene(SCENE_ID_CLEAR);
	}
	if (state->IsGameOver()) {
		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		//if (pSM != nullptr)
			//pSM->ChangeScene(SCENE_ID_GAMEOVER);
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
