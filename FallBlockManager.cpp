#include "FallBlockManager.h"
#include"Engine/Model.h"
#include"Stage.h"
#include"Engine\DeltaTime.h"
#include"Engine\Sprite.h"
#include"Engine/JsonReader.h"
#include"FallObject.h"
#include"FallBomb.h"

#include<sstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace DT = DeltaTime;
namespace JR = JsonReader;

namespace FallBlockManagerSet {
	float SPAWN_HEIGHT(0);
	//const float BLOCK_DEAD_HIGHT(-10);
	float NEXT_SPAWN_TIME(0);
	void Initialize(std::string _name) {
		JR::Get<float>(_name, "SPAWN_HEIGHT", SPAWN_HEIGHT);
		JR::Get<float>(_name, "NEXT_SPAWN_TIME", NEXT_SPAWN_TIME);
	}
}

namespace Set = FallBlockManagerSet;

FallBlockManager::FallBlockManager(GameObject* parent):
	GameObject(parent, "FallBlockManager"),time_(0)
{
}

FallBlockManager::~FallBlockManager()
{
}

void FallBlockManager::Initialize()
{
	Set::Initialize(objectName_);
	time_ = Set::NEXT_SPAWN_TIME;
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

std::vector<XMFLOAT4> FallBlockManager::GetFallingObjectCenterPosition()
{
	std::vector<XMFLOAT4> vec;
	std::list<GameObject*> objs = GetRootJob()->FindChildObjectList("FallObject");
	for (auto obj : objs) {
		FallObject* o = (FallObject*)obj;
		if (!o->IsOnGround()) {
			XMFLOAT3 p = o->GetCenterPosition();
			XMFLOAT4 pos = { p.x,p.y,p.z,0 };
			vec.push_back(pos);
		}
	}
	return vec;
}

void FallBlockManager::FallControle()
{
	time_ += DT::GetDeltaTime();

	if (time_ < Set::NEXT_SPAWN_TIME)
		return;

	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;

	time_ = 0;
	FallObject* fobj;
	if (rand() % 2 == 0)
		fobj = Instantiate<FallObject>(this);
	else
		fobj = Instantiate<FallBomb>(this);
	XMFLOAT3 pos;
	pos = stage->GetRandomScaffoldPos();
	pos.y = Set::SPAWN_HEIGHT;
	fobj->SetPosition(pos);
}
