#include "FallBomb.h"
#include "Engine\Model.h"
#include "Engine\JsonReader.h"
#include "Timer.h"

namespace JR = JsonReader;

FallBomb::FallBomb(GameObject* _parent) 
	:FallObject(_parent, "FallBomb"),explosionTimer_(nullptr)
{
}

FallBomb::~FallBomb()
{
}

void FallBomb::Initialize()
{
	hModel_ = Model::Load(JR::Get<std::string>(objectName_, "MODEL_PATH"));

	explosionTimer_ = new Timer();
}

void FallBomb::Release()
{
}

void FallBomb::Update()
{
	Fall();
	Dead();
}
