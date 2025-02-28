#include "Stage.h"
#include"Engine\Model.h"
#include"Engine\DeltaTime.h"

namespace Set {
	const XMFLOAT3 BLOCK_SIZE(1.0f, 1.0f,1.0f);
	const XMINT3 STAGE_SIZE(10, 5, 10);
	const float FALL_SPEED(5.0f);
	const float BLOCK_RETURN_HEIGHT(-10.0f);
}

namespace DT = DeltaTime;

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),hModel_(-1),hFrame_(-1)
{
}

Stage::~Stage()
{
	Release();
}

void Stage::Initialize()
{
	hModel_ = Model::Load("Assets/Model/BoxGrass.fbx");
	assert(hModel_ >= 0);
	hKazan_ = Model::Load("Assets/Model/Kazan.fbx");
	hFrame_ = Model::Load("Assets/Model/Frame/RedFrame.fbx");
	assert(hFrame_ >= 0);
	
	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		std::vector<std::vector<STAGE_BLOCK_DATA>> vec;
		for (int y = 0; y < Set::STAGE_SIZE.y; y++) {
			std::vector<STAGE_BLOCK_DATA> v;
			for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
				STAGE_BLOCK_DATA data;
				data.trans.position_ = { (float)x,(float)y,(float)z };
				if (y < 2) {
					data.type = GROUND;
					data.state = NORMAL;
				}
				else
					data.type = NONE;

				v.push_back(data);
			}
			vec.push_back(v);
		}
		blockData_.push_back(vec);
	}

	initialBlockData_ = blockData_;
}

void Stage::Update()
{
	FallStageBlock();
	ReturnBlock();
}

void Stage::Draw()
{
	Transform t;
	STAGE_BLOCK_DATA block;
	t.position_ = { (float)0,(float)-15,(float)0 };
	Model::SetTransform(hKazan_, t);
	Model::Draw(hKazan_);
	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		for (int y = 0; y < Set::STAGE_SIZE.y; y++) {
			for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
				block = blockData_[z][y][x];
				if (block.type == GROUND || block.state == FALL ||
					block.state == RETURN) {
					/*if (block.state != FALL)
						t.position_ = { (float)x,(float)y,(float)z };
					else*/
					t = block.trans;
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
				if (blockData_[z][y][x].type == GROUND) {
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
		if (blockData_[rz][y][rx].type == GROUND && blockData_[rz][y][rx].state != FALL) {
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

void Stage::SetFallBlock(int x, int y, int z)
{
	if (x >= 0 && x < Set::STAGE_SIZE.x && y >= 0 && y < Set::STAGE_SIZE.y &&
		z >= 0 && z < Set::STAGE_SIZE.z &&
		blockData_[z][y][x].type != NONE && blockData_[z][y][x].state == NORMAL) {
		//blockData_[z][y][x].type = NONE;
		blockData_[z][y][x].state = FALL;
		blockData_[z][y][x].trans.position_ = XMFLOAT3(x, y, z);
		fallBlock_.push_back(XMINT3(x,y,z));
	}
}

void Stage::DrawFrame(XMFLOAT3 _pos)
{
	Transform t;
	t.position_ = _pos;
	t.position_.x -= 0.05;
	t.position_.z -= 0.05;
	t.scale_ = { 1.1f,1.1f,1.1f };
	Model::SetTransform(hFrame_, t);
	Model::OutLineDraw(hFrame_);
}

XMINT3 Stage::GetStageSize()
{
	return Set::STAGE_SIZE;
}

XMFLOAT3 Stage::GetScaffoldPos()
{
	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
			for (int y = Set::STAGE_SIZE.y - 1; y >= 0; y--) {
				if (blockData_[z][y][x].type == GROUND) {
					return XMFLOAT3((float)x, (float)y, (float)z);
				}
			}
		}
	}
	return XMFLOAT3(0,0,0);
}

XMFLOAT3 Stage::GetRandomScaffoldPos()
{
	XMFLOAT3 pos(0,0,0);
	std::vector<XMFLOAT3> vPos;
	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
			for (int y = Set::STAGE_SIZE.y - 1; y >= 0; y--) {
				if (blockData_[z][y][x].type == GROUND) {
					vPos.push_back(XMFLOAT3((float)x, (float)y, (float)z));
				}
			}
		}
	}

	if(vPos.size() > 0)
		pos = vPos[rand() % vPos.size()];

	return pos;
}

bool Stage::GetIsOnBlock(XMINT3 _pos)
{
	if (_pos.x >= 0 && _pos.x < Set::STAGE_SIZE.x && _pos.y >= 0 && _pos.y < Set::STAGE_SIZE.y &&
		_pos.z >= 0 && _pos.z < Set::STAGE_SIZE.z) {
		if (blockData_[_pos.z][_pos.y][_pos.x].type == GROUND)
			return true;
	}
	return false;
}

bool Stage::GetHitBlockToSphere(XMFLOAT3 _pos, float _radius, XMFLOAT3& _getpos)
{
	float minLength = _radius * _radius;
	bool is = false;
	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		for (int y = 0; y < Set::STAGE_SIZE.y; y++) {
			for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
				if (blockData_[z][y][x].type == GROUND) {
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
	int y = _pos.y;

	if (y < 0 || y >= Set::STAGE_SIZE.y)
		return false;

	for (int z = 0; z < Set::STAGE_SIZE.z; z++) {
		for (int x = 0; x < Set::STAGE_SIZE.x; x++) {
			if (blockData_[z][y][x].type == GROUND) {
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

void Stage::FallStageBlock()
{
	if (fallBlock_.size() <= 0)
		return;

	std::vector<int> eraseNum;
	int count = 0;
	//—Ž‰º
	for (auto itr : fallBlock_) {
		int x = itr.x;
		int y = itr.y;
		int z = itr.z;
		blockData_[z][y][x].trans.position_.y -= Set::FALL_SPEED * DT::GetDeltaTime();
		//•œ‹A‚ÖˆÚ“®
		if (blockData_[z][y][x].trans.position_.y < Set::BLOCK_RETURN_HEIGHT) {
			blockData_[z][y][x].state = RETURN;
			blockData_[z][y][x].trans.position_ = { (float)x,(float)y,(float)z };
			blockData_[z][y][x].trans.scale_ = { 0,0,0 };
			returnBlock_.push_back(XMINT3(x, y, z));
			eraseNum.push_back(count);
		}
		count++;
	}

	//íœ
	for (auto itr : eraseNum) {
		fallBlock_.erase(fallBlock_.begin() + itr);
	}
}

void Stage::ReturnBlock()
{
	std::vector<int> eraseNum;
	int count = 0;
	for (auto itr : returnBlock_) {
		STAGE_BLOCK_DATA& block = blockData_[itr.z][itr.y][itr.x];
		block.trans.scale_.x += 0.05f;
		block.trans.scale_.y += 0.05f;
		block.trans.scale_.z += 0.05f;
		if (block.trans.scale_.x >= 1) {
			block.state = NORMAL;
			eraseNum.push_back(count);
		}
		count++;
	}

	for (auto itr : eraseNum) {
		returnBlock_.erase(returnBlock_.begin() + itr);
	}
}

