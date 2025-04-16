#include "GameSetting.h"

namespace GameSetting {
	int GAME_OVER_ON_GROUND_BLOCK_NUM(5);
}

int GameSetting::GetGameOverOnGroundBlockNum()
{
	return GAME_OVER_ON_GROUND_BLOCK_NUM;
}