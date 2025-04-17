#pragma once
#include "Engine\GameObject.h"
#include "Engine\FBX.h"

class Sprite;

class FallBlockManager :
	public GameObject
{
    int hBlock_;
    int hBomb_;
    int blockNum_;
    int nowFallBlock_;
    int onGroundBlockNum_;
    float time_;
    enum FallObject {
        BLOCK,
        BOMB,
        MAX
    };
    struct FallBlock {
        FallObject obj;
        XMFLOAT3 pos;
        bool isActive;
        bool isDead;
        bool isFall;
    };
    std::vector<FallBlock> blocks;

    bool isSetFallPos_;
    std::vector<XMINT3> fallPos_;

    //Sprite* number_[10];
    //Sprite* nokori_;
public:
    FallBlockManager(GameObject* parent);
    ~FallBlockManager();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
    int GetBlockNum();
    int GetOnGroundBlockNum();
    //void SetFallObject(int _objNum,int _bombNum);
private:
    void Fall();
    void FallControle();
    void Explosion(int _x,int _y,int _z);
    /// <summary>
    /// クラス内のブロックがクラス内のブロックとレイキャスト
    /// </summary>
    /// <param name="_rayData">RayCast用データ</param>
    /// <param name="_number">RayCastするブロック番号</param>
    void FallBlockRayCast(RayCastData& _rayData, int _number);
};

