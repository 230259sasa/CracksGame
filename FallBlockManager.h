#pragma once
#include "Engine\GameObject.h"

class Sprite;

class FallBlockManager :
	public GameObject
{
    int hModel_;
    int blockNum_;
    int nowFallBlock_;
    int onGroundBlockNum_;
    float time_;
    struct FallBlock {
        XMFLOAT3 pos;
        bool isActive_;
        bool isDead;
    };
    std::vector<FallBlock> blocks;

    bool isSetFallPos_;
    std::vector<XMINT3> fallPos_;

    Sprite* number_[10];
    Sprite* nokori_;
public:
    FallBlockManager(GameObject* parent);
    ~FallBlockManager();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
    int GetBlockNum();
    int GetOnGroundBlockNum();
private:
    void Fall();
   // void SetFallPosition();
    void FallControle();
};

