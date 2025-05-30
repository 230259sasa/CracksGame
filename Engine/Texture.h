#pragma once
#include <d3d11.h>
#include <string>
#include <wrl.h>
#include<DirectXMath.h>
//using namespace Microsoft::WRL;

class Texture
{
	ID3D11SamplerState* pSampler_;
	ID3D11ShaderResourceView* pSRV_;
	DirectX::XMFLOAT3 size_;
public:
	Texture();
	~Texture() {}
	HRESULT Load(std::string fileName);
	void Release();
	ID3D11SamplerState* GetSampler() { return pSampler_; }
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }
	DirectX::XMFLOAT3 GetSize() { return size_; }
};