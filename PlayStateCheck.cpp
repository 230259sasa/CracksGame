#include "PlayStateCheck.h"
#include "FallBlockManager.h"
#include"Engine/Sprite.h"
#include"Engine/Input.h"
#include"GameSetting.h"
#include<sstream>

namespace GS = GameSetting;

PlayStateCheck::PlayStateCheck(GameObject* parent)
	:GameObject(parent, "PlayStateCheck")
{
}

PlayStateCheck::~PlayStateCheck()
{
}

void PlayStateCheck::Initialize()
{
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
}

void PlayStateCheck::Release()
{
}

bool PlayStateCheck::IsReady()
{
	return false;
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
	FallBlockManager* fall = nullptr;
	fall = (FallBlockManager*)FindObject("FallBlockManager");
	if (fall != nullptr)
		return (fall->GetOnGroundBlockNum() >= GS::GetGameOverOnGroundBlockNum());

	return false;
}
