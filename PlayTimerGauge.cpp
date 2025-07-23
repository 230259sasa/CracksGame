#include "PlayTimerGauge.h"
#include "Engine\Sprite.h"
#include "Engine\DeltaTime.h"
#include"Engine/JsonReader.h"

namespace DT = DeltaTime;
namespace JR = JsonReader;

//namespace PlayTimerGaugeSet {
//	XMFLOAT3 GAUGE_SIZE(0, 0, 0);
//	XMFLOAT3 GAUGE_FRAME_SIZE(0, 0, 0);
//	XMFLOAT3 GAUGE_POSITION(0, 0, 0);
//	void Initialize(std::string _name) {
//		JR::Get<float>(_name, "gauge_size_x", GAUGE_SIZE.x);
//		JR::Get<float>(_name, "gauge_size_y", GAUGE_SIZE.y);
//		JR::Get<float>(_name, "gauge_frame_size_x", GAUGE_FRAME_SIZE.x);
//		JR::Get<float>(_name, "gauge_frame_size_y", GAUGE_FRAME_SIZE.y);
//		JR::Get<float>(_name, "gauge_position_x", GAUGE_POSITION.x);
//		JR::Get<float>(_name, "gauge_position_y", GAUGE_POSITION.y);
//	}
//}
//
//namespace Set = PlayTimerGaugeSet;

PlayTimerGauge::PlayTimerGauge(GameObject* parent)
	: GameObject(parent, "PlayTimerGauge"), timer_(10), initialTime_(10),
	isTimerRun_(true),gaugeFrame_(nullptr), gauge_(nullptr),
	gauge_size_({ JR::Get<float>(objectName_, "gauge_size_x"),JR::Get<float>(objectName_, "gauge_size_y"),0 }),
	gauge_frame_size_({ JR::Get<float>(objectName_, "gauge_frame_size_x"),JR::Get<float>(objectName_, "gauge_frame_size_y"),0 }),
	gauge_position_({ JR::Get<float>(objectName_, "gauge_position_x"),JR::Get<float>(objectName_, "gauge_position_y"),0 })
{
}

PlayTimerGauge::~PlayTimerGauge()
{
}

void PlayTimerGauge::Initialize()
{
	transform_.position_ = gauge_position_;
	gaugeFrame_ = new Sprite();
	gaugeFrame_->Load(JR::Get<std::string>(objectName_,"image_path_frame"));
	gauge_ = new Sprite();
	gauge_->Load(JR::Get<std::string>(objectName_, "image_path_gauge"));
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
	Transform t,tmp;
	t.position_ = transform_.position_;
	transform_.scale_ = gauge_frame_size_;
	if (timer_ > 0) {
		float n = timer_ / initialTime_;
		t.scale_ = { gauge_size_.x * n,gauge_size_.y,0 };
		t.position_.x = gauge_position_.x - gauge_size_.x * (1 - n);
		gauge_->Draw(t);
	}
	tmp.scale_ = gauge_frame_size_;
	tmp.position_ = gauge_position_;
	gaugeFrame_->Draw(tmp);
}
