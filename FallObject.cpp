#include "FallObject.h"
#include"Engine/Model.h"
#include"Engine/DeltaTime.h"
#include"Stage.h"

namespace DT = DeltaTime;

namespace Set {
	const float FALL_OBJECT_SIZE(1.0f);
	const float FALL_OBJECT_CENTER(FALL_OBJECT_SIZE / 2);
	const float FALL_SPEED(5.0f);
	const float KILL_ME_HEIGHT(-10.0f);
}

FallObject::FallObject(GameObject* parent)
	:GameObject(parent, "FallObject"),isFall_(false),isOnGround_(false),hModel_(0)
{
}

FallObject::~FallObject()
{
}

void FallObject::Initialize()
{
	hModel_ = Model::Load("Assets/Model/BoxDefault.fbx");
}

void FallObject::Release()
{
}

void FallObject::Update()
{
	Fall();
	Dead();
}

void FallObject::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void FallObject::RayCast(RayCastData& _rayData)
{
	Model::RayCast(hModel_, _rayData, transform_);
}

void FallObject::Fall()
{
	fallSpeed_ = Set::FALL_SPEED * DT::GetDeltaTime();

	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;

	RayCastData rayData, stageRayData, tmpRayData;
	XMFLOAT3 pos = transform_.position_;
	rayData.start = { pos.x + Set::FALL_OBJECT_CENTER ,pos.y,pos.z + Set::FALL_OBJECT_CENTER,0.0f };
	//////////////////////////////////////////////////////////////////
	rayData.dir = { 0,-1,0,0 };
	rayData.hit = false;
	rayData.dist = 0;
	stageRayData = rayData;
	FallObjectRayCast(rayData);
	stage->FallRayCast(stageRayData);

	if (rayData.dist < stageRayData.dist && rayData.hit)
		tmpRayData = rayData;
	else
		tmpRayData = stageRayData;

	//レイキャストが当たったかつ距離が現在のフレームの落下距離より小さいかつ落下中ならtrue
	if (tmpRayData.hit && tmpRayData.dist <= abs(fallSpeed_)) {
		transform_.position_.y -= tmpRayData.dist;
		isFall_ = false;
		isOnGround_ = true;
	}
	else {
		transform_.position_.y -= fallSpeed_;
		isFall_ = true;
		isOnGround_ = false;
	}
}

void FallObject::FallObjectRayCast(RayCastData& _rayData)
{
	std::list<GameObject*>objs = GetRootJob()->FindChildObjectList("FallObject");
	RayCastData data,minData;
	data = _rayData;
	minData = _rayData;
	minData.dist = fallSpeed_;

	for (auto obj : objs) {
		if (obj == this)
			continue;
		data = _rayData;
		FallObject* o = (FallObject*)obj;
		o->RayCast(data);
		if (data.hit && data.dist < minData.dist) {
			minData = data;
		}
	}

	_rayData = minData;
	objs.clear();
}

void FallObject::Dead()
{
	if (Set::KILL_ME_HEIGHT > transform_.position_.y) {
		KillMe();
	}
}
