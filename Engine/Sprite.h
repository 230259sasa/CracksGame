#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include <vector>
#include "Transform.h"

using std::vector;
using namespace DirectX;



class Sprite
{
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;
	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};

	uint64_t vertexNum_;
	vector<VERTEX> vertices_;
	uint64_t indexNum_;				
	vector<int> indices_;			

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;

	Texture* pTexture_;
public:
	Sprite();
	~Sprite();
	HRESULT Load(std::string fileName);
	void Draw(Transform& transform);
	void Release();
private:
	void InitVertexData();			
	HRESULT CreateVertexBuffer();

	void InitIndexData();
	HRESULT CreateIndexBuffer();

	HRESULT CreateConstantBuffer();

	HRESULT LoadTexture(std::string fileName);

	void PassDataToCB(DirectX::XMMATRIX worldMatrix);
	void SetBufferToPipeline();
};
