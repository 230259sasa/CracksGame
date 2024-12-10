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
    /// �X�e�[�W��̃u���b�N�ƃ��C�L���X�g����
    /// �����Ƃ��߂��u���b�N�Ƃ̋���������B
    /// </summary>
    /// <param name="_rayData">RayCast�p�f�[�^</param>
    void StageBlockRayCast(RayCastData& _rayData);
    XMFLOAT2 GetBlockSize();
};

