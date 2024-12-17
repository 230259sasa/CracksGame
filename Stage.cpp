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

float Stage::GetPushCenter(XMFLOAT3 _pos, float _radius)
{
	XMFLOAT3 pos = GetHitBlockToSphere(_pos, _radius);
	if (pos.x < 0)
		return 0.0f;
	float push = -(pos.z + Set::BLOCK_SIZE.z / 2 + _radius);
	push = push - pos.z;
	return -push;
}

XMFLOAT3 Stage::GetHitBlockToSphere(XMFLOAT3 _pos, float _radius)
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
						return pos;
					}
				}
			}
		}
	}
	return XMFLOAT3(-1, -1, -1);
}

float Stage::GetClosestPoint(float _bpos, float _pos)
{
	float min;
	if (_pos < _bpos - Set::BLOCK_SIZE.x / 2) {
		min = _bpos - Set::BLOCK_SIZE.x / 2;
	}
	else if (_pos > _bpos + Set::BLOCK_SIZE.x / 2) {
		min = _bpos + Set::BLOCK_SIZE.x / 2;
	}
	else {
		min = _pos;
	}

	return min;
}

