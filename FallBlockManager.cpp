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

FallBlockManager::FallBlockManager(GameObject* parent) :
	GameObject(parent, "FallBlockManager"), timer_(nullptr), isFirstBlock_(true),
	spawn_height_(JR::Get<float>(objectName_, "spawn_height")),
	next_spawn_time_(JR::Get<float>(objectName_, "next_spawn_time"))
{
}

FallBlockManager::~FallBlockManager()
{
}

void FallBlockManager::Initialize()
{
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

FallObject* FallBlockManager::GetLiftObject(XMINT3 _pos)
{
	std::vector<XMFLOAT4> vec;
	std::list<GameObject*> objs = FindAllChildObjectList();
	for (auto obj : objs) {
		FallObject* o = (FallObject*)obj;
		if (!o->IsLift() && (int)o->GetPosition().x == _pos.x &&
			(int)o->GetPosition().y == _pos.y &&
			(int)o->GetPosition().z == _pos.z) {
			o->OnLiftable();
			return o;
		}
	}
	return nullptr;
}

void FallBlockManager::FallControle()
{
	if (!timer_->IsTimeOver(next_spawn_time_) && !isFirstBlock_)
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
	pos.y = spawn_height_;
	fobj->SetPosition(pos);
}
