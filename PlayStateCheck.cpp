#include "PlayStateCheck.h"
#include "FallBlockManager.h"
#include"Engine/Sprite.h"
#include"Engine/Input.h"
#include<sstream>

namespace Set {
	const int GAME_OVER_ON_GROUND_BLOCK_NUM(5);
}

PlayStateCheck::PlayStateCheck(GameObject* parent)
	:GameObject(parent, "PlayStateCheck")
{
}

PlayStateCheck::~PlayStateCheck()
{
}

void PlayStateCheck::Initialize()
{
	for (int i = 0; i < 10; i++) {
		number_[i] = new Sprite();
		std::stringstream s;
		s << i;
		number_[i]->Load("Assets/number/" + s.str() + ".png");
	}
}

void PlayStateCheck::Update()
{
}

void PlayStateCheck::Draw()
{
	FallBlockManager* fall = nullptr;
	fall = (FallBlockManager*)FindObject("FallBlockManager");
	if (fall == nullptr)
		return;

	Transform t;
	t.scale_ = { 0.05,0.1,0.1 };
	t.position_.x = 0;
	t.position_.y = 0.8;
	int num = fall->GetOnGroundBlockNum();
	if (Set::GAME_OVER_ON_GROUND_BLOCK_NUM - num > 0)
		number_[Set::GAME_OVER_ON_GROUND_BLOCK_NUM - num]->Draw(t);
	else
		number_[0]->Draw(t);
}

void PlayStateCheck::Release()
{
}

bool PlayStateCheck::IsClear()
{
	FallBlockManager* fall = nullptr;
	fall = (FallBlockManager*)FindObject("FallBlockManager");
	if (fall != nullptr)
		return (fall->GetBlockNum() == 0);
	return false;
}

bool PlayStateCheck::IsGameOver()
{
	if (Input::IsKey(DIK_I))
		return true;

	FallBlockManager* fall = nullptr;
	fall = (FallBlockManager*)FindObject("FallBlockManager");
	if (fall != nullptr)
		return (fall->GetOnGroundBlockNum() >= Set::GAME_OVER_ON_GROUND_BLOCK_NUM);

	return false;
}
