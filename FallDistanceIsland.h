#pragma once
#include "Engine\GameObject.h"

class Sprite;

class FallDistanceIsland :
	public GameObject
{
    Sprite* Number[10];
    Sprite* DistS_;
public:
    FallDistanceIsland(GameObject* parent);
    ~FallDistanceIsland();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
};

