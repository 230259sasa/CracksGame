#include "Stage.h"
#include"Engine\Model.h"

namespace Set {
	const XMFLOAT2 BLOCK_SIZE(1.0f, 1.0f);
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
	
	for (int z = 0; z < 10; z++) {
		std::vector<std::vector<int>> vec;
		for (int y = 0; y < 10; y++) {
			std::vector<int> v;
			for (int x = 0; x < 10; x++) {
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
}

void Stage::Update()
{
}

void Stage::Draw()
{
	Transform t;
	for (int z = 0; z < 10; z++) {
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
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

	for (int z = 0; z < 10; z++) {
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
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

XMFLOAT2 Stage::GetBlockSize()
{
	return Set::BLOCK_SIZE;
}
