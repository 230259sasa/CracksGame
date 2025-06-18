#include "PlayTimerGauge.h"
#include "Engine\Sprite.h"
#include "Engine\DeltaTime.h"

namespace Set {
	const XMFLOAT3 GAUGE_SIZE(0.285, 0.07, 0);
	const XMFLOAT3 GAUGE_FRAME_SIZE(0.3, 0.1, 0);
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
	transform_.scale_ = Set::GAUGE_FRAME_SIZE;
	if (timer_ > 0) {
		float n = timer_ / initialTime_;
		t.scale_ = { Set::GAUGE_SIZE.x * n,Set::GAUGE_SIZE.y,0 };
		t.position_.x = Set::GAUGE_POSITION.x - Set::GAUGE_SIZE.x * (1 - n);
		gauge_->Draw(t);
	}

	gaugeFrame_->Draw(transform_);
}
