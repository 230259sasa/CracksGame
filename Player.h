#pragma once
#include "Engine\GameObject.h"
class Player :
    public GameObject
{
    int hModel_;
public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
};

