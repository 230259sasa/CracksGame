#pragma once
#include"Engine\GameObject.h"

class Sprite;
class FallBlockManager;

class FallIslandManager :
    public GameObject
{
    const float fall_speed_;
    const XMFLOAT3 frame_size_;
    const XMFLOAT3 frame_positoin_;
    const XMFLOAT3 island_size_;
    const XMFLOAT3 island_position_;
    Transform islandTrans_;
    Sprite* frame_;
    Sprite* island_;
    FallBlockManager* fbManager_;
    float percentage_;
    float tmpPercentage_;
public:
	FallIslandManager(GameObject* parent);
	~FallIslandManager();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
    float GetPercentage() { return percentage_; }
private:
    void RatioCalculation();
    void Fall();
};
