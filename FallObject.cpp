#include "FallObject.h"
#include"Engine/Model.h"
#include"Engine/DeltaTime.h"
#include"Engine/JsonReader.h"
#include"Stage.h"

namespace DT = DeltaTime;
namespace JR = JsonReader;
using std::string;

namespace FallObjectSet {
	float FALL_OBJECT_SIZE(0);
	float FALL_OBJECT_CENTER(0);//XMFLOAT3にする？
	float FALL_SPEED(0);
	float FALL_LIMIT_HEIGHT(0);
	float RAY_CAST_INITIALIZE_DISTANCE(0);
	XMFLOAT4 RAY_CAST_DIR(0, 0, 0, 0);
	void Initialize(std::string _name) {
		JR::Get<float>(_name, "OBJECT_SIZE", FALL_OBJECT_SIZE);
		JR::Get<float>(_name, "FALL_SPEED", FALL_SPEED);
		JR::Get<float>(_name, "FALL_LIMIT", FALL_LIMIT_HEIGHT);
		JR::Get<float>(_name, "RAY_CAST_INITIALIZE_DISTANCE", RAY_CAST_INITIALIZE_DISTANCE);
		JR::Get<float>(_name, "RAY_CAST_DIR_X", RAY_CAST_DIR.x);
		JR::Get<float>(_name, "RAY_CAST_DIR_Y", RAY_CAST_DIR.y);
		JR::Get<float>(_name, "RAY_CAST_DIR_Z", RAY_CAST_DIR.z);
		FALL_OBJECT_CENTER = FALL_OBJECT_SIZE / 2;
	}
}

namespace Set = FallObjectSet;

FallObject::FallObject(GameObject* _parent, string _name)
	:GameObject(_parent, _name), isFall_(false), isOnGround_(false), hModel_(0)
{
	Set::Initialize(objectName_);
}

FallObject::FallObject(GameObject* _parent)
	:GameObject(_parent, "FallObject"),isFall_(false),isOnGround_(false),hModel_(0)
{
	Set::Initialize(objectName_);
}

FallObject::~FallObject()
{
}

void FallObject::Initialize()
{
	hModel_ = Model::Load(JR::Get<string>(objectName_,"MODEL_PATH"));
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

XMFLOAT3 FallObject::GetCenterPosition()
{
	return { transform_.position_.x + Set::FALL_OBJECT_CENTER,
		transform_.position_.y + Set::FALL_OBJECT_CENTER,
		transform_.position_.z + Set::FALL_OBJECT_CENTER };
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
	rayData.dir = Set::RAY_CAST_DIR;
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
	minData.dist = Set::RAY_CAST_INITIALIZE_DISTANCE;

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
	if (Set::FALL_LIMIT_HEIGHT > transform_.position_.y) {
		KillMe();
	}
}
