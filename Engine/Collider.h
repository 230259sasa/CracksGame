#pragma once
#include <DirectXMath.h>
using namespace DirectX;

enum ColliderType
{
	COLLIDER_BOX,		//” Œ^
	COLLIDER_CIRCLE		//‹…‘Ì
};

class GameObject;
class SphereCollider;
class BoxCollider;

class Collider
{
protected:
	GameObject* pGameObject_;
	ColliderType	type_;
	XMFLOAT3		center_;
	XMFLOAT3		size_;
public:
	Collider();
	~Collider();
	virtual bool IsHit(Collider* _target);
	bool IsHitBoxToBox(BoxCollider* _boxA, BoxCollider* _boxB);
	bool IsHitBoxToSphere(BoxCollider* _box, SphereCollider* _sphere);
	bool IsHitSphereToSphere(SphereCollider* _sphereA, SphereCollider* _sphereB);
	XMFLOAT3 GetPosition();
	XMFLOAT3 AddXMFloat3(const XMFLOAT3 A,const XMFLOAT3 B);
};

