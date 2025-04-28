#include "FallObject.h"
#include"Engine/Model.h"
#include"Engine/DeltaTime.h"
#include"Stage.h"

namespace DT = DeltaTime;

namespace Set {
	const float FALL_SPEED(5);
}

FallObject::FallObject(GameObject* parent)
	:GameObject(parent, "FallObject"),isDead_(false),isFall_(false),
	isGround_(false),hModel_(0)
{
}

FallObject::~FallObject()
{
}

void FallObject::Initialize()
{
}

void FallObject::Release()
{
}

void FallObject::Update()
{
}

void FallObject::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void FallObject::Fall()
{
	/*if (!isFall_)
		return;*/

	float fallSpeed = Set::FALL_SPEED * DT::GetDeltaTime();

	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;

	RayCastData rayData, stageRayData, tmpRayData;
	XMFLOAT3 pos = transform_.position_;
	rayData.start = { pos.x ,pos.y,pos.z,0.0f };
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
	if (tmpRayData.hit && tmpRayData.dist <= abs(fallSpeed)) {
		transform_.position_.y -= tmpRayData.dist;
		isFall_ = false;
	}
	else {
		transform_.position_.y -= fallSpeed;
		isFall_ = true;
	}
}

bool FallObject::FallObjectRayCast(RayCastData& _rayData)
{
	////////
	Model::RayCast(hModel_, _rayData, transform_);
}