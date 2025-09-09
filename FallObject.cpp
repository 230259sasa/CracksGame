#include "FallObject.h"
#include"Engine/Model.h"
#include"Engine/DeltaTime.h"
#include"Engine/JsonReader.h"
#include"Stage.h"

namespace DT = DeltaTime;
namespace JR = JsonReader;
using std::string;

namespace FallObjectSet {
	//float FALL_OBJECT_SIZE(0);
	//float FALL_OBJECT_CENTER(0);//XMFLOAT3にする？
	//float FALL_SPEED(0);
	//float FALL_LIMIT_HEIGHT(0);
	//float RAY_CAST_INITIALIZE_DISTANCE(0);
	//XMFLOAT4 RAY_CAST_DIR(0, 0, 0, 0);
	/*void Initialize(std::string _name) {
		JR::Get<float>(_name, "object_size", FALL_OBJECT_SIZE);
		JR::Get<float>(_name, "fall_speed", FALL_SPEED);
		JR::Get<float>(_name, "fall_limit", FALL_LIMIT_HEIGHT);
		JR::Get<float>(_name, "ray_cast_initialize_distance", RAY_CAST_INITIALIZE_DISTANCE);
		JR::Get<float>(_name, "ray_cast_dir_x", RAY_CAST_DIR.x);
		JR::Get<float>(_name, "ray_cast_dir_y", RAY_CAST_DIR.y);
		JR::Get<float>(_name, "ray_cast_dir_z", RAY_CAST_DIR.z);
		FALL_OBJECT_CENTER = FALL_OBJECT_SIZE / 2;
	}*/
}

namespace Set = FallObjectSet;

FallObject::FallObject(GameObject* _parent, string _name)
	:GameObject(_parent, _name), isFall_(false), isOnGround_(false), hModel_(0),
	fall_object_center_(JR::Get<float>("FallObject", "object_size")/2),
	fall_speed_(JR::Get<float>("FallObject", "fall_speed")),
	fall_limit_height_(JR::Get<float>("FallObject", "fall_limit")),
	initial_ray_cast_distance_(1),
	ray_cast_dir_({0,-1.0f,0,0})
{
}

FallObject::FallObject(GameObject* _parent)
	:FallObject(_parent, "FallObject")
{
}

FallObject::~FallObject()
{
}

void FallObject::Initialize()
{
	hModel_ = Model::Load(JR::Get<string>("FallObject","model_path"));
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
	return { transform_.position_.x + fall_object_center_,
		transform_.position_.y + fall_object_center_,
		transform_.position_.z + fall_object_center_ };
}

void FallObject::RayCast(RayCastData& _rayData)
{
	Model::RayCast(hModel_, _rayData, transform_);
}

void FallObject::Fall()
{
	fallSpeed_ = fall_speed_ * DT::GetDeltaTime();

	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;

	RayCastData rayData, stageRayData, tmpRayData;
	XMFLOAT3 pos = transform_.position_;
	rayData.start = { pos.x + fall_object_center_ ,pos.y,pos.z + fall_object_center_,0.0f };
	rayData.dir = ray_cast_dir_;
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
	minData.dist = initial_ray_cast_distance_;

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
	if (fall_limit_height_ > transform_.position_.y) {
		KillMe();
	}
}
