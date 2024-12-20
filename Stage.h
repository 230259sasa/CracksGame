#pragma once
#include "Engine\GameObject.h"
#include "Engine\FBX.h"
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
    /// <summary>
    /// ステージ上のブロックとレイキャストして
    /// もっとも近いブロックとの距離を入れる。
    /// </summary>
    /// <param name="_rayData">RayCast用データ</param>
    void StageBlockRayCast(RayCastData& _rayData);
    XMFLOAT3 GetBlockSize();
    XMFLOAT3 GetPushBack(XMFLOAT3 _pos, float _radius, XMFLOAT3 _dir);
private:
    //関数名を変える
    //当たったブロックとの距離を返す
    bool GetHitBlockToSphere(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos);
    //球と長方形の最短点を求める
    float GetClosestPoint(float _bpos,float _pos);
};

