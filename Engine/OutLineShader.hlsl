//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

Texture2D g_toon_texture : register(t1); //�e�N�X�`���[
SamplerState g_toon_sampler : register(s1); //�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global : register(b0)
{
    //�ϊ��s��A���_�A����
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW;
    float4x4 matNormal; //�@�������[���h���W�ɑΉ�������s�񁁉�]��
    float4 diffuseColor; //�g�U���ˌW��
    //float4 lightVec; //���s�����̃x�N�g��
    float4 factor; //diffuseFactor
    float4 ambientColor;
    float4 specularColor;
    float4 shininess;
    bool isTextured; //tex���\���Ă��邩
};

cbuffer gStage : register(b1)
{
    float4 lightVec;
    float4 eyePosition;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 cos_alpha : COLOR; //�F�i���邳�j
    float4 normal : NORMAL;
    float4 eyev : POSITION1;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
float4 VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL):SV_Position
{
    float4 outPos;
    
    normal.w = 0;
    normal = normalize(normal);
    outPos = pos + normal * 0.1;
    
    outPos = mul(outPos, matWVP);

    return outPos;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    return float4(0, 0, 0, 1.0);
}