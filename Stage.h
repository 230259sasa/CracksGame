#pragma once
#include "Engine\GameObject.h"
#include "Engine\FBX.h"

class Stage :
    public GameObject
{
    enum STAGE_BLOCK {
        NONE = 0,
        NORMAL
    };
    struct STAGE_BLOCK_DATA {
        bool isOutLineDraw;
        STAGE_BLOCK block;
    };
    int hModel_;
    int hFrame_;
    std::vector<std::vector<std::vector<int>>> stage_;
    std::vector<std::vector<std::vector<STAGE_BLOCK_DATA>>> blockData_;
public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    /// <summary>
    /// ステージ上のブロックとレイキャストして
    /// もっとも近いブロックとの距離を入れる。
    /// </summary>
    /// <param name="_rayData">RayCast用データ</param>
    void StageBlockRayCast(RayCastData& _rayData);
    void FallRayCast(RayCastData& _rayData);
    XMFLOAT3 GetBlockSize();
    XMFLOAT3 GetPushBack(XMFLOAT3 _pos, float _radius);
    void SetBlock(int x, int y, int z);
    void SetNoneBlock(int x, int y, int z);
    void DrawFrame(XMFLOAT3 _pos);
    XMINT3 GetStageSize();
    XMFLOAT3 GetScaffoldPos();
    XMFLOAT3 GetRandomScaffoldPos();
    bool GetIsOnBlock(XMINT3 _pos);
private:
    //関数名を変える
    //当たったブロックとの距離を返す
    bool GetHitBlockToSphere(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos);
    bool GetHitBlockToCircle(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos);
    //球と長方形の最短点を求める
    float GetClosestPoint(float _bpos,float _pos);
};

