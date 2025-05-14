#pragma once
#include"Transform.h"
#include <dInput.h>
#include"Xinput.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")
#pragma comment(lib, "Xinput.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Input {
	void Initialize(HWND hWnd);
	void Update();
	//キーボード
	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyUp(int keyCode);
	void Release();
	//マウス
	XMVECTOR GetMousePosition();
	void SetMousePosition(int x, int y);
	bool IsMouseButton(int buttonCode);
	bool IsMouseButtonDown(int buttonCode);
	bool IsMouseButtonUp(int buttonCode);
	//コントローラー
	bool IsPadButton(int buttonCode, int padID);
	bool IsPadButtonDown(int buttonCode, int padID);
	bool IsPadButtonUp(int buttonCode, int padID);
	XMFLOAT3 GetPadStickL(int padID);
	XMFLOAT3 GetPadStickR(int padID);
	float GetPadTrrigerL(int padID);
	float GetPadTrrigerR(int padID);
	void SetPadVibration(int motorL, int motorR, int padID);
}