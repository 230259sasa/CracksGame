#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <assert.h>
#include"../imgui/imgui.h"
#include"../imgui/imgui_impl_dx11.h"
#include"../imgui/imgui_impl_win32.h"

enum SHADER_TYPE {
	SHADER_2D,SHADER_3D,SHADER_OUTLINE,SHADER_MAX
};
enum BLEND_TYPE {
	BLEND_INVALID,BLEND_VALID,BLEND_MAX
};
//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Direct3D

{
	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);
	//シェーダー準備
	HRESULT InitShader();
	HRESULT InitShader3D();
	HRESULT InitShader2D();
	HRESULT InitShaderOutLine();
	void SetShader(SHADER_TYPE type);
	void SetBlend(BLEND_TYPE type);
	//描画開始
	void BeginDraw();
	//描画終了
	void EndDraw();
	//解放
	void Release();

	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;

};