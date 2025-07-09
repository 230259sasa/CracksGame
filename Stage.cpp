#include "Stage.h"
#include"Engine\Model.h"
#include"Engine\DeltaTime.h"
#include"Engine/JsonReader.h"
#include"Player.h"
#include"FallBlockManager.h"
#include"FallObject.h"

namespace JR = JsonReader;
namespace DT = DeltaTime;

namespace StageSet {
	XMFLOAT3 BLOCK_SIZE(0, 0, 0);
	XMINT3 STAGE_SIZE(0, 0, 0);
	float FALL_SPEED(0);
	float FALL_LIMIT(0);
	float RETURN_BLOCK_SPEED(0);
	const int  MAX_SHADOW_OBJECTS(2);
	const float SOFTNESS(3.0f);
	void Initialize(std::string _name) {
		float size = JR::Get<float>(_name, "BLOCK_SIZE");
		BLOCK_SIZE = { size,size,size };
		JR::Get<int>(_name, "STAGE_SIZE_X", STAGE_SIZE.x);
		JR::Get<int>(_name, "STAGE_SIZE_Y", STAGE_SIZE.y);
		JR::Get<int>(_name, "STAGE_SIZE_Z", STAGE_SIZE.z);
		JR::Get<float>(_name, "FALL_LIMIT", FALL_LIMIT);
		JR::Get<float>(_name, "RETURN_BLOCK_SPEED", RETURN_BLOCK_SPEED);
		JR::Get<float>(_name, "FALL_SPEED", FALL_SPEED);
	}
}

namespace Set = StageSet;

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
	Set::Initialize(objectName_);
	hModel_ = Model::Load(JR::Get<std::string>(objectName_,"MODEL_PATH_GRASS"));
	assert(hModel_ >= 0);
	Model::SetShaderType(hModel_, SHADER_STAGE);

	hKazan_ = Model::Load("Kazan.fbx");
	hFrame_ = Model::Load(JR::Get<std::string>(objectName_, "MODEL_PATH_FRAME"));
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

	//////
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTBUFFER_STAGE);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
	}
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

	CONSTBUFFER_STAGE cb;
	Player* player = (Player*)FindObject("Player");
	if(player == nullptr)
		return;
	FallBlockManager* fManager = (FallBlockManager*)FindObject("FallBlockManager");
	if (fManager == nullptr)
		return;
	std::vector<XMFLOAT4> objsPos = fManager->GetFallingObjectPosition();

	XMFLOAT3 pPos = player->GetPosition();
	cb.casterPos = { pPos.x,pPos.y,pPos.z,0 };
	cb.shadowParams = { Set::SOFTNESS,0,0,pPos.y - GetTerrainHeight(pPos.x,pPos.z)};
	for (int i = 0; i < Set::MAX_SHADOW_OBJECTS; i++) {
		cb.objPos[i] = { 0,0,0,0 };
		cb.shadowObjParams[i] = { 0, 0, 0, 0};
	}
	for (int i = 0; i < std::min<int>(Set::MAX_SHADOW_OBJECTS, (int)objsPos.size()); i++) {
		XMFLOAT4 pos = objsPos[i];
		if (pos.y < 0)
			break;
		float sizeY = JR::Get<float>("FallObject", "OBJECT_SIZE");
		pos.x += sizeY / 2;
		pos.z += sizeY / 2;
		cb.objPos[i] = pos;
		cb.shadowObjParams[i] = { Set::SOFTNESS, 0, 0, pos.y - GetTerrainHeight(pos.x, pos.z) };
	}

	//
	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開
	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(1, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(1, 1, &pConstantBuffer_);	//ピクセルシェーダー

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

void Stage::ChangeBlockTypeNone(int x, int y, int z)
{
	if (x >= 0 && x < Set::STAGE_SIZE.x && y >= 0 && y < Set::STAGE_SIZE.y &&
		z >= 0 && z < Set::STAGE_SIZE.z &&
		blockData_[z][y][x].type != NONE && blockData_[z][y][x].state == NORMAL) {
		blockData_[z][y][x].type = NONE;
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

float Stage::GetTerrainHeight(int _x, int _z)
{
	if (_x >= 0 && _x < Set::STAGE_SIZE.x &&_z >= 0 && _z < Set::STAGE_SIZE.z) {
		for (int y = Set::STAGE_SIZE.y - 1;y >= 0;y--) {
			if (blockData_[_x][y][_z].type == STAGE_BLOCK_TYPE::GROUND)
				return y + Set::BLOCK_SIZE.y;
		}
	}
	return 0.0f;
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
			if (blockData_[z][y][x].type == GROUND &&
				blockData_[z][y][x].state == NORMAL) {
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
	//落下
	for (auto itr : fallBlock_) {
		int x = itr.x;
		int y = itr.y;
		int z = itr.z;
		blockData_[z][y][x].trans.position_.y -= Set::FALL_SPEED * DT::GetDeltaTime();
		//落下終了の高さ(-)+初期の高さ
		float height = Set::FALL_LIMIT + y;
		if (blockData_[z][y][x].trans.position_.y < height) {
			blockData_[z][y][x].state = RETURN;
			blockData_[z][y][x].trans.position_ = { (float)x,(float)y,(float)z };
			blockData_[z][y][x].trans.scale_ = { 0,0,0 };
			returnBlock_.push_back(XMINT3(x, y, z));
			eraseNum.push_back(count);
		}
		count++;
	}

	//削除
	for (auto itr = eraseNum.rbegin(); itr != eraseNum.rend(); ++itr) {
		fallBlock_.erase(fallBlock_.begin() + *itr);
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

	//削除
	for (auto itr = eraseNum.rbegin(); itr != eraseNum.rend(); ++itr) {
		returnBlock_.erase(returnBlock_.begin() + *itr);
	}
}

