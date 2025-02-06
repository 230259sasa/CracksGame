#include "PlaySceneBackground.h"
#include"Engine/Model.h"

PlaySceneBackground::PlaySceneBackground(GameObject* parent)
	:GameObject(parent, "PlaySceneBackground"),hModel_(-1)
{
}

void PlaySceneBackground::Initialize()
{
	hModel_ = Model::Load("Assets/Model/PlaySceneBackground.fbx");
	transform_.position_.z = 25;
}

void PlaySceneBackground::Update()
{
}

void PlaySceneBackground::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void PlaySceneBackground::Release()
{
}
