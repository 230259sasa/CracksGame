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
    /// ブロックのサイズより遠い位置でのhitは
    /// falseを入れられる。
    /// </summary>
    /// <param name="_rayData">RayCast用データ</param>
    void StageBlockRayCast(RayCastData& _rayData);
    XMFLOAT2 GetBlockSize();
};

