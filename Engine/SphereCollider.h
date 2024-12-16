#pragma once
#include "Collider.h"

class SphereCollider
	:public Collider
{
public:
	SphereCollider(float _radius);
	float GetRadius() { return size_.x; }
};

