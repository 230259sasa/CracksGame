#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "../Player.h"

using namespace DirectX;

//-----------------------------------------------------------
//カメラ
//-----------------------------------------------------------
namespace Camera
{
	//初期化（プロジェクション行列作成）
	void Initialize();
	void Initialize(XMVECTOR _position, XMVECTOR _target);

	//更新（ビュー行列作成）
	void Update();

	//視点（カメラの位置）を設定
	void SetPosition(XMVECTOR _position);

	//焦点（見る位置）を設定
	void SetTarget(XMVECTOR _target);

	void SetPlayerPointer(Player* _player);

	//ビュー行列を取得
	XMMATRIX GetViewMatrix();

	//プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix();

	//位置を取得
	XMVECTOR GetPosition();

	//焦点を取得
	XMVECTOR GetTarget();

	void RotateCameraLeft();

	void RotateCameraRight();
};