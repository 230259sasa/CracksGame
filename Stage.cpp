#include "Stage.h"
#include"Engine\Model.h"

namespace Set {
	const XMFLOAT3 BLOCK_SIZE(1.0f, 1.0f,1.0f);
	const XMFLOAT3 STAGE_SIZE(10, 10, 10);
}

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),hModel_(-1)
{
}

Stage::~Stage()
{
	Release();
}

void Stage::Initialize()
{
	hModel_ = Model::Load("Assets/Model/BoxDefault.fbx");
	assert(hModel_ >= 0);
	
	for (int z = 0; z < Set::STAGE_SIZE.x; z++) {
		std::vector<std::vector<int>> vec;
		for (int y = 0; y < Set::STAGE_SIZE.y; y++) {
			std::vector<int> v;
			for (int x = 0; x < Set::STAGE_SIZE.z; x++) {
				if(y<1)
					v.push_back(1);
				else
					v.push_back(0);
			}
			vec.push_back(v);
		}
		stage.push_back(vec);
	}
	stage[0][1][3] = 1;
	stage[0][1][4] = 1;
	stage[0][1][5] = 1;
	stage[1][1][3] = 1;
	stage[1][1][4] = 1;
	stage[1][1][5] = 1;

	stage[2][1][3] = 1;
	stage[2][1][4] = 1;
	stage[2][1][5] = 1;
	stage[2][2][3] = 1;
	stage[2][2][4] = 1;
	stage[2][2][5] = 1;

	stage[3][1][3] = 1;
	stage[3][1][4] = 1;
	stage[3][1][5] = 1;
	stage[3][2][3] = 1;
	stage[3][2][4] = 1;
	stage[3][2][5] = 1;
	stage[3][3][3] = 1;
	stage[3][3][4] = 1;
	stage[3][3][5] = 1;

	stage[4][1][3] = 1;
	stage[4][1][4] = 1;
	stage[4][1][5] = 1;
	stage[4][2][3] = 1;
	stage[4][2][4] = 1;
	stage[4][2][5] = 1;
	stage[4][3][3] = 1;
	stage[4][3][4] = 1;
	stage[4][3][5] = 1;
	stage[4][4][3] = 1;
	stage[4][4][4] = 1;
	stage[4][4][5] = 1;

	stage[7][1][5] = 1;
}

void Stage::Update()
{
}

void Stage::Draw()
{
	Transform t;
	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		for (int y = 0; y < Set::STAGE_SIZE.y; y++) {
			for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
				if (stage[z][y][x] == 1) {
					t.position_ = { (float)x,(float)y,(float)z};
					Model::SetTransform(hModel_, t);
					Model::Draw(hModel_);
				}
			}
		}
	}
}

void Stage::Release()
{
}

void Stage::StageBlockRayCast(RayCastData& _rayData)
{
	Transform t;
	RayCastData data = _rayData;
	RayCastData minDistData = data;
	minDistData.dist = Set::BLOCK_SIZE.x;

	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		for (int y = 0; y < Set::STAGE_SIZE.y; y++) {
			for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
				if (stage[z][y][x] == 1) {
					t.position_ = { (float)x,(float)y,(float)z };
					Model::RayCast(hModel_, data, t);
					if (data.hit && data.dist < minDistData.dist) {
						minDistData = data;
					}
				}
			}
		}
	}
	_rayData = minDistData;
}

XMFLOAT3 Stage::GetBlockSize()
{
	return Set::BLOCK_SIZE;
}

XMFLOAT3 Stage::GetPushBack(XMFLOAT3 _pos, float _radius, XMFLOAT3 _dir)
{
	

	//移動量をもとめる
	//push.x = (移動量) * -_dir.x;
	//push.z = (移動量) * -_dir.z;

	//push.x = pos.x +  (-_dir.x * _radius) - _pos.x;
	//push.z = pos.z + (-_dir.z * _radius) - _pos.z;
	XMFLOAT3 pos;
	XMFLOAT3 push(0, 0, 0);
	//衝突位置を求める
	if (!GetHitBlockToSphere(_pos, _radius, pos))
		return push;

	//押し出し後も接触しないように足している
	//_radius += 0.01f;
	
	//衝突位置から
	//float x = pos.x + sqrt(_radius * _radius - temp.z * temp.z);
	//float z = pos.z + sqrt(_radius * _radius - temp.x * temp.x);
	//衝突位置+半径*衝突位置から衝突しない位置への方向ベクトル = 押し出し後の位置
	XMFLOAT3 temp(0, 0, 0);
	temp.x = pos.x + (_radius * ベクトル);
	temp.z = pos.z + (_radius * ベクトル);
	//押し出し後の位置-現在の位置 = 移動量(x,y,z)
	temp.x -= _pos.x;
	temp.z -= _pos.z;
	//x座標,z座標から移動量を求める
	float move = sqrt(temp.x * temp.x + temp.z * temp.z);
	//移動量rから反転した方向ベクトルをかける
	push.x = move * -_dir.x;
	push.z = move * -_dir.z;
	return push;
}

bool Stage::GetHitBlockToSphere(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos)
{
	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		for (int y = 0; y < Set::STAGE_SIZE.y; y++) {
			for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
				if (stage[z][y][x] == 1) {
					XMFLOAT3 pos = { (float)x,(float)y,(float)z };
					XMFLOAT3 min;
					min.x = GetClosestPoint(pos.x, _pos.x);
					min.y = GetClosestPoint(pos.y, _pos.y);
					min.z = GetClosestPoint(pos.z, _pos.z);
					XMFLOAT3 len;
					len.x = _pos.x - min.x;
					len.y = _pos.y - min.y;
					len.z = _pos.z - min.z;
					float length = len.x * len.x + len.y * len.y + len.z * len.z;
					if (length <= _radius * _radius) {
						_getpos = min;
						return true;
					}
				}
			}
		}
	}
	return false;
}

float Stage::GetClosestPoint(float _bpos, float _pos)
{
	float min = _pos;
	if (_pos < _bpos - Set::BLOCK_SIZE.x / 2) {
		min = _bpos - Set::BLOCK_SIZE.x / 2;
	}
	else if (_pos > _bpos + Set::BLOCK_SIZE.x / 2) {
		min = _bpos + Set::BLOCK_SIZE.x / 2;
	}

	return min;
}

