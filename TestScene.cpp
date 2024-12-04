#include "TestScene.h"
#include"Engine/Input.h"
#include"Engine\SceneManager.h"
#include"Stage.h"
#include"Player.h"

TestScene::TestScene(GameObject* parent)
	:GameObject(parent,"TestScene")
{
}

void TestScene::Initialize()
{
	//tex = new Sprite();
	//tex->Load("Assets\\texture.png");
	//transform_.scale_ = { 0.2,0.2,0.2 };   
	Instantiate<Stage>(this);
	Instantiate<Player>(this);
}

void TestScene::Update()
{
	/*if (Input::IsKeyDown(DIK_SPACE)) {
		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM != nullptr)
		pSM->ChangeScene(SCENE_ID_PLAY);
	}*/
}

void TestScene::Draw()
{
	//tex->Draw(transform_);
}

void TestScene::Release()
{
}
