#pragma once
#include"Engine\GameObject.h"

class Sprite;

class FallIslandManager :
    public GameObject
{
    Transform islandTrans_;
    Sprite* frame_;
    Sprite* island_;
public:
	FallIslandManager(GameObject* parent);
	~FallIslandManager();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
};

