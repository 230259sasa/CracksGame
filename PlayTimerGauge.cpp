#include "PlayTimerGauge.h"
#include "Engine\Sprite.h"
#include "Engine\DeltaTime.h"
#include "Engine/Display.h"

namespace Set {
	const XMFLOAT3 GAUGE_POSITION(0, 0.85f, 0);
}

namespace DT = DeltaTime;

PlayTimerGauge::PlayTimerGauge(GameObject* parent)
	: GameObject(parent, "PlayTimerGauge"), timer_(10), initialTime_(10),
	isTimerRun_(true),gaugeFrame_(nullptr), gauge_(nullptr)
{
}

PlayTimerGauge::~PlayTimerGauge()
{
}

void PlayTimerGauge::Initialize()
{
	transform_.position_ = Set::GAUGE_POSITION;
	gaugeFrame_ = new Sprite();
	gaugeFrame_->Load("Assets\\Image\\GaugeFrame.png");
	gauge_ = new Sprite();
	gauge_->Load("Assets\\Image\\Gauge.png");
}

void PlayTimerGauge::Release()
{
	gaugeFrame_->Release();
	delete gaugeFrame_;
	gauge_->Release();
	delete gauge_;
}

void PlayTimerGauge::Update()
{
	if (timer_ > 0 && isTimerRun_) {
		timer_ -= DT::GetDeltaTime();
	}
}

void PlayTimerGauge::Draw()
{
	Transform t;
	t.position_ = transform_.position_;
	if (timer_ > 0) {
		float n = timer_ / initialTime_;
		t.scale_ = { n,1,0 };
		t.position_.x = Set::GAUGE_POSITION.x - ((gauge_->GetSize().x) * (1-n))/Display::GetWindowWidth();
	}

	gauge_->Draw(t);
	gaugeFrame_->Draw(transform_);
}
