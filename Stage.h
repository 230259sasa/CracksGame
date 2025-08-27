#pragma once
#include "Engine\GameObject.h"
#include "Engine\FBX.h"

class Stage :
    public GameObject
{
    enum STAGE_BLOCK_TYPE {
        NONE = 0,
        GROUND
    };
    enum STAGE_BLOCK_STATE {
        NORMAL,
        FALL,
        RETURN
    };
    struct STAGE_BLOCK_DATA {
        STAGE_BLOCK_TYPE type;
        STAGE_BLOCK_STATE state;
        Transform trans;
    };
    struct CONSTBUFFER_STAGE {
        XMFLOAT4 casterPos;     // プレイヤーのXZ
        XMFLOAT4 shadowParams;  // (softness, alphaScale, unused, playerHeightY)
        XMFLOAT4 objPos[2];
        XMFLOAT4 shadowObjParams[2];
    };
    const XMINT3 stage_size_;
    const float block_size_;
    //
    const float fall_speed_;
    const float fall_limit_;
    //
    const float shadow_softness_;
    const int max_shadow_objects_;
    ID3D11Buffer* pConstantBuffer_;
    int hModel_;
    int hKazan_;//クラスを作る
    int hFrame_;
    std::vector<std::vector<std::vector<STAGE_BLOCK_DATA>>> initialBlockData_;
    std::vector<std::vector<std::vector<STAGE_BLOCK_DATA>>> blockData_;
    std::vector<XMINT3> fallBlock_;
    std::vector<XMINT3> returnBlock_;
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
    void BreakBlock(int x, int y, int z);
    void SetFallBlock(int x, int y, int z);
    void ChangeBlockTypeNone(int x, int y, int z);
    void DrawFrame(XMFLOAT3 _pos);
    XMINT3 GetStageSize();
    XMFLOAT3 GetScaffoldPos();
    XMFLOAT3 GetRandomScaffoldPos();
    bool GetIsOnBlock(XMINT3 _pos);
private:
    float GetTerrainHeight(int _x, int _z);
    //当たったブロックとの距離を返す
    bool GetHitBlockToSphere(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos);
    bool GetHitBlockToCircle(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos);
    //球と長方形の最短点を求める
    float GetClosestPoint(float _bpos,float _pos);
    void FallStageBlock();
    void ReturnBlock();
    bool IsInStageSize(int x, int y, int z);
};

