#include "Collider.h"
#include "GameObject.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

Collider::Collider()
{
}

Collider::~Collider()
{
}

bool Collider::IsHit(Collider* target)
{
    return false;
}

bool Collider::IsHitBoxToBox(BoxCollider* _boxA, BoxCollider* _boxB)
{
	XMFLOAT3 APos = AddXMFloat3(_boxA->GetPosition(), _boxA->center_);
	XMFLOAT3 BPos = AddXMFloat3(_boxB->GetPosition(), _boxB->center_);

	if ((APos.x + _boxA->size_.x / 2) > (BPos.x - _boxB->size_.x / 2) &&
		(APos.x - _boxA->size_.x / 2) < (BPos.x + _boxB->size_.x / 2) &&
		(APos.y + _boxA->size_.y / 2) > (BPos.y - _boxB->size_.y / 2) &&
		(APos.y - _boxA->size_.y / 2) < (BPos.y + _boxB->size_.y / 2) &&
		(APos.z + _boxA->size_.z / 2) > (BPos.z - _boxB->size_.z / 2) &&
		(APos.z - _boxA->size_.z / 2) < (BPos.z + _boxB->size_.z / 2))
	{
		return true;
	}
	return false;
}

bool Collider::IsHitBoxToSphere(BoxCollider* _box, SphereCollider* _sphere)
{
	XMFLOAT3 cPos = AddXMFloat3(_sphere->pGameObject_->GetPosition(), _sphere->center_);
	XMFLOAT3 bPos = AddXMFloat3(_box->pGameObject_->GetPosition(), _box->center_);

	if (cPos.x > bPos.x - _box->size_.x - _sphere->size_.x &&
		cPos.x < bPos.x + _box->size_.x + _sphere->size_.x &&
		cPos.y > bPos.y - _box->size_.y - _sphere->size_.x &&
		cPos.y < bPos.y + _box->size_.y + _sphere->size_.x &&
		cPos.z > bPos.z - _box->size_.z - _sphere->size_.x &&
		cPos.z < bPos.z + _box->size_.z + _sphere->size_.x)
	{
		return true;
	}
	return false;
}

bool Collider::IsHitSphereToSphere(SphereCollider* _sphereA, SphereCollider* _sphereB)
{
	XMFLOAT3 pos = AddXMFloat3(_sphereA->GetPosition(), _sphereA->center_);
	XMVECTOR APos = XMLoadFloat3(&pos);
	pos = AddXMFloat3(_sphereB->GetPosition(), _sphereB->center_);
	XMVECTOR BPos = XMLoadFloat3(&pos);
	XMVECTOR d = XMVector3Length(APos - BPos);
	float dist = XMVectorGetX(d);
	//��̔��a�����킹���l
	float rDist = _sphereA->size_.x + _sphereB->size_.x;
	if (dist <= rDist) {
		return true;
	}

	return false;
}

XMFLOAT3 Collider::GetPosition()
{
	return pGameObject_->GetPosition();
}

XMFLOAT3 Collider::AddXMFloat3(XMFLOAT3 A, XMFLOAT3 B)
{
	XMFLOAT3 temp;
	temp.x = A.x + B.x;
	temp.y = A.y + B.y;
	temp.z = A.z + B.z;
	return temp;
}
