#pragma once
#include "Collider.h"
class BoxCollider :
    public Collider
{
public:
    BoxCollider(XMFLOAT3 _size);
};

