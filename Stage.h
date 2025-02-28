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
    int hModel_;
    int hKazan_;//�N���X�����
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
    /// �X�e�[�W��̃u���b�N�ƃ��C�L���X�g����
    /// �����Ƃ��߂��u���b�N�Ƃ̋���������B
    /// </summary>
    /// <param name="_rayData">RayCast�p�f�[�^</param>
    void StageBlockRayCast(RayCastData& _rayData);
    void FallRayCast(RayCastData& _rayData);
    XMFLOAT3 GetBlockSize();
    XMFLOAT3 GetPushBack(XMFLOAT3 _pos, float _radius);
    void SetFallBlock(int x, int y, int z);
    void DrawFrame(XMFLOAT3 _pos);
    XMINT3 GetStageSize();
    XMFLOAT3 GetScaffoldPos();
    XMFLOAT3 GetRandomScaffoldPos();
    bool GetIsOnBlock(XMINT3 _pos);
private:
    //�֐�����ς���
    //���������u���b�N�Ƃ̋�����Ԃ�
    bool GetHitBlockToSphere(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos);
    bool GetHitBlockToCircle(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos);
    //���ƒ����`�̍ŒZ�_�����߂�
    float GetClosestPoint(float _bpos,float _pos);
    void FallStageBlock();
    void ReturnBlock();
};

