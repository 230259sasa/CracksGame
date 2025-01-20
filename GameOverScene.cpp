#include "GameOverScene.h"
#include "Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Engine/Sprite.h"

GameOverScene::GameOverScene(GameObject* parent)
	:GameObject(parent, "GameOverScene")
{
}

void GameOverScene::Initialize()
{
	tex_ = new Sprite();
	tex_->Load("Assets/GAME_OVER.png");
}

void GameOverScene::Update()
{
	if (Input::IsKeyDown(DIK_J)) {
		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM != nullptr)
			pSM->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	tex_->Draw(transform_);
}

void GameOverScene::Release()
{
}
