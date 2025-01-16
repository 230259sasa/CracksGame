#include "FallDistanceIsland.h"
#include "Engine\Sprite.h"

FallDistanceIsland::FallDistanceIsland(GameObject* parent) :
	GameObject(parent, "Player"), DistS_(nullptr)
{
}

FallDistanceIsland::~FallDistanceIsland()
{
}

void FallDistanceIsland::Initialize()
{
	for (int i = 0; i < 10; i++) {
		Number[i] = new Sprite();
	}
	DistS_ = new Sprite();
}

void FallDistanceIsland::Release()
{
}

void FallDistanceIsland::Update()
{
}

void FallDistanceIsland::Draw()
{
}
