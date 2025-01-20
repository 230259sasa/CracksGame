#include "ClearScene.h"
#include "Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Engine/Sprite.h"

ClearScene::ClearScene(GameObject* parent)
	:GameObject(parent, "ClearScene")
{
}

void ClearScene::Initialize()
{
	tex_ = new Sprite();
	tex_->Load("Assets/CLEAR.png");
}

void ClearScene::Update()
{
	if (Input::IsKeyDown(DIK_J)) {
		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM != nullptr)
			pSM->ChangeScene(SCENE_ID_TITLE);
	}
}

void ClearScene::Draw()
{
	tex_->Draw(transform_);
}

void ClearScene::Release()
{
}
