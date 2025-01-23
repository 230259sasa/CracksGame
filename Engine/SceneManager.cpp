#include "SceneManager.h"
#include"../TestScene.h"
#include"../TitleScene.h"
#include"../PlayScene.h"
#include"../ClearScene.h"
#include"../GameOverScene.h"
#include"Direct3D.h"
#include"Model.h"

SceneManager::SceneManager(GameObject* parent)
	:GameObject(parent,"SceneManager")
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID_PLAY;
	nextSceneID_ = currentSceneID_;
	Instantiate<PlayScene>(this);
}

void SceneManager::Update()
{
	if (nextSceneID_ != currentSceneID_) {
		auto delScene = childList_.begin();
		(*delScene)->ReleaseSub();
		SAFE_DELETE(*delScene);
		childList_.clear();
		Model::Release();

		switch (nextSceneID_)
		{
		case SCENE_ID_TEST:Instantiate<TestScene>(this); break;
		case SCENE_ID_TITLE:Instantiate<TitleScene>(this); break;
		case SCENE_ID_PLAY:Instantiate<PlayScene>(this); break;
		case SCENE_ID_CLEAR:Instantiate<ClearScene>(this); break;
		case SCENE_ID_GAMEOVER:Instantiate<GameOverScene>(this); break;
		default:break;
		}
		currentSceneID_ = nextSceneID_;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}
