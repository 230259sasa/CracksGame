#include "TitleScene.h"
#include "Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Engine/Sprite.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent, "TitleScene")
{
}

void TitleScene::Initialize()
{
	tex_ = new Sprite();
	tex_->Load("Assets/TITLE.png");
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_J)) {
		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM != nullptr)
			pSM->ChangeScene(SCENE_ID_PLAY);
	}
}

void TitleScene::Draw()
{
	tex_->Draw(transform_);
}

void TitleScene::Release()
{
}
