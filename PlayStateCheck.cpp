#include "PlayStateCheck.h"
#include "FallBlockManager.h"
#include"Engine/Sprite.h"
#include"Engine/Input.h"
#include"GameSetting.h"
#include"PlayTimerGauge.h"
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
	PlayTimerGauge* t = nullptr;
	t = (PlayTimerGauge*)FindObject("PlayTimerGauge");
	if (t != nullptr)
		return t->IsTimeOver();
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
