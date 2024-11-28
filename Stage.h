#pragma once
#include "Engine\GameObject.h"

class Stage :
    public GameObject
{
    int hModel_;
    std::vector<std::vector<std::vector<int>>> stage;
public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

