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
	const int MAX_FALL_BLOCK(20);
	const float DEFAULT_HIGHT(10);
	const float BLOCK_DEAD_HIGHT(-10);
	const float BLOCK_SIZE(1);

	const int ACTIVE_BLOCK_NUM(1);
	const int GAME_OVER_ON_GROUND_BLOCK_NUM(8);
	const float NEXT_FALL_TIME(2.5f);
}

namespace DT = DeltaTime;

FallBlockManager::FallBlockManager(GameObject* parent):
	GameObject(parent, "FallBlockManager"),hModel_(-1),blockNum_(0),
	time_(0),nowFallBlock_(0),isSetFallPos_(false),onGroundBlockNum_(0)
{
}

FallBlockManager::~FallBlockManager()
{
}

void FallBlockManager::Initialize()
{
	hModel_ = Model::Load("Assets/Model/BoxDefault.fbx");
	assert(hModel_ >= 0);

	for (int i = 0; i < Set::MAX_FALL_BLOCK; i++) {
		XMFLOAT3 pos = { 0,Set::DEFAULT_HIGHT,0 };
		FallBlock b;
		b.pos = pos;
		b.isActive = false;
		b.isDead = false;
		blocks.push_back(b);
	}

	for (int i = 0; i < 10; i++) {
		number_[i] = new Sprite();
		std::stringstream s;
		s << i;
		number_[i]->Load("Assets/number/" + s.str() + ".png");
	}

	nokori_ = new Sprite();
	nokori_->Load("Assets/number/nokori.png");

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
		Model::SetTransform(hModel_,t);
		Model::Draw(hModel_);
	}

	Transform t;
	t.scale_ = { 0.1,0.1,0.1 };
	t.position_.x = -0.9;
	t.position_.y = 0.8;
	nokori_->Draw(t);
	t.position_.x += 0.2;
	t.scale_.x = 0.07;
	number_[blockNum_ / 10]->Draw(t);
	t.position_.x += 0.15;
	number_[blockNum_ % 10]->Draw(t);

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
	float fallSpeed = 5 * DT::GetDeltaTime();

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

//void FallBlockManager::SetFallPosition()
//{
//	if (isSetFallPos_)
//		return;
//
//	Stage* stage = nullptr;
//	stage = (Stage*)FindObject("Stage");
//	if (stage == nullptr)
//		return;
//
//	int stageX = stage->GetStageSize().x;
//	int stageZ = stage->GetStageSize().z;
//	std::vector<std::vector<bool>> map;
//	for (int z = 0; z < stageZ; z++) {
//		std::vector<bool> is;
//		for (int x = 0; x < stageX; x++) {
//			is.push_back(false);
//		}
//		map.push_back(is);
//	}
//
//	for (int z = 0; z < stageZ; z++) {
//		for (int x = 0; x < stageX; x++) {
//			if (z % 2 == 0 || x % 2 == 0)
//				continue;
//
//			map[z][x] = true;
//			int n;
//			if (z == 1)
//				n = rand() % 4;
//			else
//				n = rand() % 3;
//			//上
//			if (n == 0) {
//				if (!map[z + 1][x])
//					map[z + 1][x] = true;
//				else
//					n++;
//			}
//			//右
//			if (n == 1) {
//				if (!map[z][x + 1])
//					map[z][x + 1] = true;
//				else
//					n++;
//			}
//			//左
//			if (n == 2) {
//				if (!map[z][x - 1])
//					map[z][x - 1] = true;
//				else
//					n++;
//			}
//			//下
//			if (n == 3) {
//				if (!map[z - 1][x])
//					map[z - 1][x] = true;
//				else
//					continue;
//			}
//		}
//	}
//
//	for (int z = 0; z < stageZ; z++) {
//		for (int x = 0; x < stageX; x++) {
//			if (map[z][x])
//				fallPos_.push_back({ x,(int)Set::DEFAULT_HIGHT,z });
//		}
//	}
//
//	for (int i = 0; i < fallPos_.size(); i++) {
//		XMINT3 tmp = fallPos_[i];
//		int num = rand() % fallPos_.size();
//		fallPos_[i] = fallPos_[num];
//		fallPos_[num] = tmp;
//	}
//
//	isSetFallPos_ = true;
//}

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
		Model::RayCast(hModel_, data, t);
		if (data.hit && data.dist < minDistData.dist) {
			minDistData = data;
		}
	}

	_rayData = minDistData;
}
