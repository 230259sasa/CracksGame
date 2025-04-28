#pragma once
#include"Engine/GameObject.h"
#include"Engine/FBX.h"

class FallObject
	:GameObject
{
	int hModel_;
	bool isDead_;
	bool isFall_;
	bool isGround_;
public:
	FallObject(GameObject* parent);
	~FallObject();
	virtual void Initialize() override;
	virtual void Release() override;
	virtual void Update() override;
	void Draw() override;
	void SetPosition(XMFLOAT3 _pos) { transform_.position_ = _pos; }
	void DeadObject() { isDead_ = true; }
	void StartFall() { isFall_ = true; }
	void StopFall() { isFall_ = false; }
private:
	void Fall();
	bool FallObjectRayCast(RayCastData& _rayData);
};

