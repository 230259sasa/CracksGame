#include "FallBlockManager.h"
#include"Engine/Model.h"
#include"Stage.h"
#include"Engine\DeltaTime.h"
#include"Engine\Sprite.h"

#include<sstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace Set {
	const int MAX_FALL_BLOCK(40);
	const int EXPLOSION_RANGE(1);
	const float DEFAULT_HIGHT(10);
	const float BLOCK_DEAD_HIGHT(-10);
	const float BLOCK_SIZE(1);
	const float FALL_SPEED(5);

	const int ACTIVE_BLOCK_NUM(1);
	const int GAME_OVER_ON_GROUND_BLOCK_NUM(8);
	const float NEXT_FALL_TIME(2.5f);
}

namespace DT = DeltaTime;

FallBlockManager::FallBlockManager(GameObject* parent):
	GameObject(parent, "FallBlockManager"),hBlock_(-1),hBomb_(-1),blockNum_(0),
	time_(0),nowFallBlock_(0),isSetFallPos_(false),onGroundBlockNum_(0)
{
}

FallBlockManager::~FallBlockManager()
{
}

void FallBlockManager::Initialize()
{
	hBlock_ = Model::Load("Assets/Model/BoxDefault.fbx");
	assert(hBlock_ >= 0);
	hBomb_ = Model::Load("Assets/Model/Bomb.fbx");
	assert(hBomb_ >= 0);

	for (int i = 0; i < Set::MAX_FALL_BLOCK; i++) {
		XMFLOAT3 pos = { 0,Set::DEFAULT_HIGHT,0 };
		FallBlock b;
		b.pos = pos;
		b.isActive = false;
		b.isDead = false;
		if (rand() % 2 == 0)
			b.obj = BLOCK;
		else
			b.obj = BOMB;
		blocks.push_back(b);
	}

	/*for (int i = 0; i < 10; i++) {
		number_[i] = new Sprite();
		std::stringstream s;
		s << i;
		number_[i]->Load("Assets/number/" + s.str() + ".png");
	}

	nokori_ = new Sprite();
	nokori_->Load("Assets/number/nokori.png");*/

	blockNum_ = Set::MAX_FALL_BLOCK;
}

void FallBlockManager::Release()
{
}

void FallBlockManager::Update()
{
	//SetFallPosition();
	Fall();
	FallControle();
}

void FallBlockManager::Draw()
{
	for (int i = 0; i < Set::MAX_FALL_BLOCK; i++) {
		if (!blocks[i].isActive || blocks[i].isDead)
			continue;
		Transform t;
		t.position_ = blocks[i].pos;
		int model=0;
		if (blocks[i].obj == BLOCK) {
			model = hBlock_;
		}
		else if (blocks[i].obj == BOMB) {
			model = hBomb_;
		}
		Model::SetTransform(model,t);
		Model::Draw(model);
	}

	/*Transform t;
	t.scale_ = { 0.1,0.1,0.1 };
	t.position_.x = -0.9;
	t.position_.y = 0.8;
	nokori_->Draw(t);
	t.position_.x += 0.2;
	t.scale_.x = 0.07;
	number_[blockNum_ / 10]->Draw(t);
	t.position_.x += 0.15;
	number_[blockNum_ % 10]->Draw(t);*/

	/*t.position_.x = 0;
	if (Set::GAME_OVER_ON_GROUND_BLOCK_NUM - onGroundBlockNum_ > 0)
		number_[Set::GAME_OVER_ON_GROUND_BLOCK_NUM - onGroundBlockNum_]->Draw(t);
	else
		number_[0]->Draw(t);*/

	//{
	//	ImGui::Text("block->%3.0li",blockNum_);
	//}
}

int FallBlockManager::GetBlockNum()
{
	return blockNum_;
}

int FallBlockManager::GetOnGroundBlockNum()
{
	return onGroundBlockNum_;
}

void FallBlockManager::Fall()
{
	//ステージ上のブロックとレイキャスト
	//atode teisuu ni
	float fallSpeed = Set::FALL_SPEED * DT::GetDeltaTime();

	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;

	onGroundBlockNum_ = 0;
	for (int i = 0; i < Set::MAX_FALL_BLOCK; i++) {
		if (!blocks[i].isActive || blocks[i].isDead)
			continue;
		if (blocks[i].pos.y <= Set::BLOCK_DEAD_HIGHT) {
			blocks[i].isDead = true;
			blockNum_--;
			continue;
		}

		RayCastData rayData,stageRayData,tmpRayData;
		XMFLOAT3 pos = blocks[i].pos;
		rayData.start = { pos.x + Set::BLOCK_SIZE / 2,pos.y,pos.z + Set::BLOCK_SIZE / 2,0.0f };
		rayData.dir = { 0,-1,0,0 };
		rayData.hit = false;
		rayData.dist = 0;
		stageRayData = rayData;
		FallBlockRayCast(rayData,i);
		stage->FallRayCast(stageRayData);

		if (rayData.dist < stageRayData.dist && rayData.hit)
			tmpRayData = rayData;
		else
			tmpRayData = stageRayData;

		//レイキャストが当たったかつ距離が現在のフレームの落下距離より小さいかつ落下中ならtrue
		if (tmpRayData.hit && tmpRayData.dist <= abs(fallSpeed)) {
			blocks[i].pos.y -= tmpRayData.dist;
			blocks[i].isFall = false;
			onGroundBlockNum_++;
			continue;
		}
		else {
			blocks[i].pos.y -= fallSpeed;
			blocks[i].isFall = true;
		}
	}
}

void FallBlockManager::FallControle()
{
	time_ += DT::GetDeltaTime();

	if (time_ < Set::NEXT_FALL_TIME)
		return;

	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;

	time_ = 0;
	for (int i = nowFallBlock_; (i < nowFallBlock_ + Set::ACTIVE_BLOCK_NUM)&&(i<Set::MAX_FALL_BLOCK);i++) {
		if (blocks[i].isDead)
			continue;
		/*if (i >= fallPos_.size()) {
			continue;
		}
		else {
			blocks[i].isActive_ = true;
			blocks[i].pos = XMFLOAT3(fallPos_[i].x, fallPos_[i].y, fallPos_[i].z);
		}*/
		blocks[i].isActive = true;
		
		blocks[i].pos = stage->GetRandomScaffoldPos();
		blocks[i].pos.y = Set::DEFAULT_HIGHT;
	}

	nowFallBlock_ += Set::ACTIVE_BLOCK_NUM;
}

void FallBlockManager::Explosion(int _x, int _y, int _z)
{
	Stage* stage = nullptr;
	stage = (Stage*)FindObject("Stage");
	if (stage == nullptr)
		return;

	int tmpX = _x - Set::EXPLOSION_RANGE;
	int tmpZ = _z + Set::EXPLOSION_RANGE;
	for (int x = tmpX; x <= _x + Set::EXPLOSION_RANGE; x++) {
		for (int y = _y; y >= 0; y--) {
			for (int z = tmpZ; z >= _z - Set::EXPLOSION_RANGE; z--) {
				stage->ChangeBlockTypeNone(x, y, z);
			}
		}
	}
}

void FallBlockManager::FallBlockRayCast(RayCastData& _rayData, int _number)
{
	Transform t;
	RayCastData data = _rayData;
	RayCastData minDistData = data;
	minDistData.dist = Set::BLOCK_SIZE;

	for (int i = 0; i < Set::MAX_FALL_BLOCK; i++) {
		if (!blocks[i].isActive || blocks[i].isDead || i == _number || blocks[i].isFall)
			continue;
		t.position_ = blocks[i].pos;
		Model::RayCast(hBlock_, data, t);
		if (data.hit && data.dist < minDistData.dist) {
			minDistData = data;
		}
	}

	_rayData = minDistData;
}
