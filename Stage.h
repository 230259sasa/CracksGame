#pragma once
#include "Engine\GameObject.h"
#include "Engine\FBX.h"
class Stage :
    public GameObject
{
    enum STAGE_BLOCK {
        NONE = 0,
        NORMAL,
        NAIL
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
    /// �X�e�[�W��̃u���b�N�ƃ��C�L���X�g����
    /// �����Ƃ��߂��u���b�N�Ƃ̋���������B
    /// </summary>
    /// <param name="_rayData">RayCast�p�f�[�^</param>
    void StageBlockRayCast(RayCastData& _rayData);
    void FallRayCast(RayCastData& _rayData);
    XMFLOAT3 GetBlockSize();
    XMFLOAT3 GetPushBack(XMFLOAT3 _pos, float _radius);
    void SetBlock(int x, int y, int z);
private:
    //�֐�����ς���
    //���������u���b�N�Ƃ̋�����Ԃ�
    bool GetHitBlockToSphere(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos);
    bool GetHitBlockToCircle(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos);
    //���ƒ����`�̍ŒZ�_�����߂�
    float GetClosestPoint(float _bpos,float _pos);
};

