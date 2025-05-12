#include "FallBomb.h"
#include "Engine\Model.h"

FallBomb::FallBomb(GameObject* _parent) 
	:FallObject(_parent, "FallObject")
{
}

FallBomb::~FallBomb()
{
}

void FallBomb::Initialize()
{
	hModel_ = Model::Load("Assets/Model/Bomb.fbx");
}

void FallBomb::Release()
{
}

void FallBomb::Update()
{
	Fall();
	Dead();
}
