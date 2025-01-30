#pragma once
#include "Engine\GameObject.h"
#include "Engine\FBX.h"

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
        bool isActive;
        bool isDead;
        bool isFall;
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
    /// <summary>
    /// クラス内のブロックがクラス内のブロックとレイキャスト
    /// </summary>
    /// <param name="_rayData">RayCast用データ</param>
    /// <param name="_number">RayCastするブロック番号</param>
    void FallBlockRayCast(RayCastData& _rayData, int _number);
};

