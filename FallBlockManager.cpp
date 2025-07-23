#include "FallBlockManager.h"
#include"Engine/Model.h"
#include"Stage.h"
#include"Engine\DeltaTime.h"
#include"Engine\Sprite.h"
#include"Engine/JsonReader.h"
#include"FallObject.h"
#include"FallBomb.h"
#include"Timer.h"

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
		JR::Get<float>(_name, "spawn_height", SPAWN_HEIGHT);
		JR::Get<float>(_name, "next_spawn_time", NEXT_SPAWN_TIME);
	}
}

namespace Set = FallBlockManagerSet;

FallBlockManager::FallBlockManager(GameObject* parent) :
	GameObject(parent, "FallBlockManager"), timer_(nullptr), isFirstBlock_(true)
{
}

FallBlockManager::~FallBlockManager()
{
}

void FallBlockManager::Initialize()
{
	Set::Initialize(objectName_);
	timer_ = new Timer();
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

void FallBlockManager::StopSpawnBlock()
{
	timer_->StopTimer();
}

void FallBlockManager::StartSpawnBlock()
{
	timer_->StartTimer();
}

int FallBlockManager::GetOnGroundBlockNum()
{
	int num = 0;
	std::list<GameObject*> objs = FindAllChildObjectList();

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
	std::list<GameObject*> objs = FindAllChildObjectList();
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
	if (!timer_->IsTimeOver(Set::NEXT_SPAWN_TIME) && !isFirstBlock_)
		return;
	isFirstBlock_ = false;
	timer_->ResetTimer();
	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;

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
