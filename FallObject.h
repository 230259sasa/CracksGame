#pragma once
#include"Engine/GameObject.h"
#include"Engine/FBX.h"

class FallObject
	:public GameObject
{
private:
	const float fall_object_center_;
	const float fall_speed_;
	const float fall_limit_height_;
	const float initial_ray_cast_distance_;
	const XMFLOAT4 ray_cast_dir_;
	float fallSpeed_;
	bool isFall_;
	bool isOnGround_;

	XMFLOAT3 throwDir_;
	float throwDist_;
protected:
	int hModel_;
	bool isLift_;
public:
	FallObject(GameObject* _parent,std::string _name);
	FallObject(GameObject* _parent);
	~FallObject();
	virtual void Initialize() override;
	virtual void Release() override;
	virtual void Update() override;
	void Draw() override;
	void SetPosition(XMFLOAT3 _pos) { transform_.position_ = _pos; }
	XMFLOAT3 GetCenterPosition();
	//void StartFall() { isFall_ = true; }
	//void StopFall() { isFall_ = false; }
	bool IsOnGround() { return isOnGround_; }
	bool IsLift() { return isLift_; }
	void RayCast(RayCastData& _rayData);
	void OnLiftable();
	void OnThrow();
	void OnThrow(XMFLOAT3 _dir, float _dist);
protected:
	void Fall();
	void ThrowMove();
	void Dead();
private:
	void FallObjectRayCast(RayCastData& _rayData);
};

