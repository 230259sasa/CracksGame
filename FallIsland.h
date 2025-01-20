#pragma once
#include "Engine\GameObject.h"

class Sprite;

class FallIsland :
	public GameObject
{
public:
    FallIsland(GameObject* parent);
    ~FallIsland();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
};

