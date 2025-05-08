#include "FallBlockManager.h"
#include"Engine/Model.h"
#include"Stage.h"
#include"Engine\DeltaTime.h"
#include"Engine\Sprite.h"
#include"FallObject.h"

#include<sstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace Set {
	const float DEFAULT_HIGHT(10);
	//const float BLOCK_DEAD_HIGHT(-10);
	const float NEXT_FALL_TIME(2.5f);
}

namespace DT = DeltaTime;

FallBlockManager::FallBlockManager(GameObject* parent):
	GameObject(parent, "FallBlockManager"),time_(0)
{
}

FallBlockManager::~FallBlockManager()
{
}

void FallBlockManager::Initialize()
{
}

void FallBlockManager::Release()
{
}

void FallBlockManager::Update()
{
	FallControle();
}

void FallBlockManager::Draw()
{
}

int FallBlockManager::GetOnGroundBlockNum()
{
	int num = 0;
	std::list<GameObject*> objs = GetRootJob()->FindChildObjectList("FallObject");

	for (auto obj : objs) {
		FallObject* o = (FallObject*)obj;
		if (o->IsOnGround())
			num++;
	}

	return num;
}

void FallBlockManager::FallControle()
{
	time_ += DT::GetDeltaTime();

	if (time_ < Set::NEXT_FALL_TIME)
		return;

	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;

	time_ = 0;
	FallObject* fobj;
	fobj = Instantiate<FallObject>(this);
	XMFLOAT3 pos;
	pos = stage->GetRandomScaffoldPos();
	pos.y = Set::DEFAULT_HIGHT;
	pos.x = 0;
	fobj->SetPosition(pos);
}
