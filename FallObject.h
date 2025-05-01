#pragma once
#include"Engine/GameObject.h"
#include"Engine/FBX.h"

class FallObject
	:public GameObject
{
private:
	float fallSpeed_;
	bool isFall_;
	bool isOnGround_;
protected:
	int hModel_;
public:
	FallObject(GameObject* parent);
	~FallObject();
	virtual void Initialize() override;
	virtual void Release() override;
	virtual void Update() override;
	void Draw() override;
	void SetPosition(XMFLOAT3 _pos) { transform_.position_ = _pos; }
	//void StartFall() { isFall_ = true; }
	//void StopFall() { isFall_ = false; }
	bool IsOnGround() { return isOnGround_; }
	void RayCast(RayCastData& _rayData);
private:
	void Fall();
	void FallObjectRayCast(RayCastData& _rayData);
	void Dead();
};

