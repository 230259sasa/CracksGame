#include "Stage.h"
#include"Engine\Model.h"

namespace Set {
	const XMFLOAT3 BLOCK_SIZE(1.0f, 1.0f,1.0f);
	const XMFLOAT3 STAGE_SIZE(20, 5, 20);
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
	hFrame_ = Model::Load("Assets/Model/Frame/RedBigFrame.fbx");
	assert(hModel_ >= 0);
	
	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		std::vector<std::vector<STAGE_BLOCK_DATA>> vec;
		for (int y = 0; y < Set::STAGE_SIZE.y; y++) {
			std::vector<STAGE_BLOCK_DATA> v;
			for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
				STAGE_BLOCK_DATA data;
				data.isOutLineDraw = false;
				if(y<1)
					data.block = NORMAL;
				else
					data.block = NONE;
				v.push_back(data);
			}
			vec.push_back(v);
		}
		blockData_.push_back(vec);
	}

	blockData_[0][1][3].block = NORMAL;
	/*blockData_[0][1][4].block = NORMAL;
	blockData_[0][1][5].block = NORMAL;
	blockData_[1][1][3].block = NORMAL;
	blockData_[1][1][4].block = NORMAL;
	blockData_[1][1][5].block = NORMAL;

	blockData_[2][1][3].block = NORMAL;
	blockData_[2][1][4].block = NORMAL;
	blockData_[2][1][5].block = NORMAL;
	blockData_[2][2][3].block = NORMAL;
	blockData_[2][2][4].block = NORMAL;
	blockData_[2][2][5].block = NORMAL;

	blockData_[3][1][3].block = NORMAL;
	blockData_[3][1][4].block = NORMAL;
	blockData_[3][1][5].block = NORMAL;
	blockData_[3][2][3].block = NORMAL;
	blockData_[3][2][4].block = NORMAL;
	blockData_[3][2][5].block = NORMAL;
	blockData_[3][3][3].block = NORMAL;
	blockData_[3][3][4].block = NORMAL;
	blockData_[3][3][5].block = NORMAL;

	blockData_[4][1][3].block = NORMAL;
	blockData_[4][1][4].block = NORMAL;
	blockData_[4][1][5].block = NORMAL;
	blockData_[4][2][3].block = NORMAL;
	blockData_[4][2][4].block = NORMAL;
	blockData_[4][2][5].block = NORMAL;
	blockData_[4][3][3].block = NORMAL;
	blockData_[4][3][4].block = NORMAL;
	blockData_[4][3][5].block = NORMAL;
	blockData_[4][4][3].block = NORMAL;
	blockData_[4][4][4].block = NORMAL;
	blockData_[4][4][5].block = NORMAL;

	blockData_[7][1][5].block = NORMAL;*/
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
				if (blockData_[z][y][x].block == NORMAL) {
					t.position_ = { (float)x,(float)y,(float)z};
					Model::SetTransform(hModel_, t);
					Model::Draw(hModel_);
					if (blockData_[z][y][x].isOutLineDraw) {
						t.position_ = { (float)x,(float)y + 0.2f,(float)z };
						Model::SetTransform(hModel_, t);
						Model::OutLineDraw(hModel_);
					}
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
				if (blockData_[z][y][x].block == NORMAL) {
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

void Stage::FallRayCast(RayCastData& _rayData)
{
	Transform t;
	RayCastData data = _rayData;
	RayCastData minDistData = data;
	minDistData.dist = Set::BLOCK_SIZE.x;

	int rx = (int)_rayData.start.x;
	int rz = (int)_rayData.start.z;
	if (rx < 0 || rx >= Set::STAGE_SIZE.x || rz < 0 || rz >= Set::STAGE_SIZE.z)
		return;
	for (int y = 0; y < Set::STAGE_SIZE.y; y++) {
		if (blockData_[rz][y][rx].block == NORMAL) {
			t.position_ = { (float)rx,(float)y,(float)rz };
			Model::RayCast(hModel_, data, t);
			if (data.hit && data.dist < minDistData.dist) {
				minDistData = data;
			}
		}
	}
	
	_rayData = minDistData;
}


XMFLOAT3 Stage::GetBlockSize()
{
	return Set::BLOCK_SIZE;
}

XMFLOAT3 Stage::GetPushBack(XMFLOAT3 _pos, float _radius)
{
	XMFLOAT3 pos(0, 0, 0);
	XMFLOAT3 push(0, 0, 0);

	_radius += 0.02;
	_pos.x += push.x;
	_pos.z += push.z;

	if (!GetHitBlockToCircle(_pos, _radius, pos))
		return push;

	float x = pos.x - _pos.x;
	float z = pos.z - _pos.z;
	float length = x * x + z * z;
	float dirX = 0;
	float dirZ = 0;
	if (x != 0) {
		dirX = (x * x) / length;
		if (x > 0)
			dirX = -dirX;
	}
	if (z != 0) {
		dirZ = (z * z) / length;
		if (z > 0)
			dirZ = -dirZ;
	}
	float move = (_radius * _radius) - length;
	push.x += move * dirX;
	push.z += move * dirZ;

	return push;
}

void Stage::SetBlock(int x, int y, int z)
{
	if (x >= 0 && x < Set::STAGE_SIZE.x && y >= 0 && y < Set::STAGE_SIZE.y && z >= 0 && z < Set::STAGE_SIZE.z) {
		blockData_[z][y][x].block = NORMAL;
	}
}

bool Stage::GetHitBlockToSphere(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos)
{
	float minLength = _radius * _radius;
	bool is = false;
	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		for (int y = 0; y < Set::STAGE_SIZE.y; y++) {
			for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
				if (blockData_[z][y][x].block == NORMAL) {
					XMFLOAT3 pos = { (float)x,(float)y,(float)z };
					XMFLOAT3 min;
					min.x = GetClosestPoint(pos.x + Set::BLOCK_SIZE.x / 2, _pos.x);
					min.y = GetClosestPoint(pos.y + Set::BLOCK_SIZE.y / 2, _pos.y);
					min.z = GetClosestPoint(pos.z - Set::BLOCK_SIZE.z / 2, _pos.z);
					XMFLOAT3 len;
					len.x = _pos.x - min.x;
					len.y = _pos.y - min.y;
					len.z = _pos.z - min.z;
					float length = len.x * len.x + len.y * len.y + len.z * len.z;
					if (length <= _radius * _radius && length < minLength) {
						_getpos = min;
						minLength = length;
						is = true;
					}
				}
			}
		}
	}
	return is;
}

bool Stage::GetHitBlockToCircle(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos)
{
	float minLength = _radius + _radius;
	bool is = false;
	//­”Ø‚èã‚°
	/*if (_pos.y - (int)_pos.y > 0)
		_pos.y += 1;*/
	int y = _pos.y;

	//
	if (y < 0 || y >= Set::STAGE_SIZE.y)
		return is;

	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
			if (blockData_[z][y][x].block == NORMAL) {
				XMFLOAT3 pos = { (float)x,(float)y,(float)z };
				XMFLOAT3 min;
				min.x = GetClosestPoint(pos.x, _pos.x);
				min.z = GetClosestPoint(pos.z, _pos.z);
				XMFLOAT3 len;
				len.x = _pos.x - min.x;
				len.z = _pos.z - min.z;
				float length = len.x * len.x + len.z * len.z;
				if (length <= _radius * _radius && length < minLength) {
					_getpos = min;
					minLength = length;
					is = true;
				}
			}
		}
	}
	return is;
}

float Stage::GetClosestPoint(float _bpos, float _pos)
{
	float min = _pos;
	if (_pos < _bpos) {
		min = _bpos;
	}
	else if (_pos > _bpos + Set::BLOCK_SIZE.x) {
		min = _bpos + Set::BLOCK_SIZE.x;
	}

	return min;
}

