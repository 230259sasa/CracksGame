#include "Sprite.h"
#include "Camera.h"
#include <filesystem>



Sprite::Sprite()
	:pTexture_(nullptr), pVertexBuffer_(nullptr),
	pIndexBuffer_(nullptr), pConstantBuffer_(nullptr),
	vertexNum_(0), indexNum_(0)
{
	blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ZeroMemory(&BlendDesc_, sizeof(BlendDesc_));
	BlendDesc_.AlphaToCoverageEnable = FALSE;
	BlendDesc_.IndependentBlendEnable = FALSE;
	BlendDesc_.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc_.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc_.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;;
	BlendDesc_.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc_.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc_.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc_.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

Sprite::~Sprite()
{
	Release();
}

HRESULT Sprite::Load(std::string fileName)
{
	InitVertexData();
	if (FAILED(CreateVertexBuffer()))
	{
		return E_FAIL;
	}

	InitIndexData();
	if (FAILED(CreateIndexBuffer()))
	{
		return E_FAIL;
	}

	if (FAILED(CreateConstantBuffer()))
	{
		return E_FAIL;
	}

	if (FAILED(LoadTexture(fileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}


void Sprite::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_TYPE::SHADER_2D);
	
	transform.Calculation();

	PassDataToCB(transform.GetWorldMatrix());
	SetBufferToPipeline();

	//透過
	//float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	/*ID3D11BlendState* mBlendState;

	D3D11_BLEND_DESC BlendDesc;*/
	

	Direct3D::pDevice->CreateBlendState(&BlendDesc_, &mBlendState_);
	Direct3D::pContext->OMSetBlendState(mBlendState_, blendFactor, 0xffffffff);
	Direct3D::pContext->DrawIndexed(indexNum_, 0, 0);
}

void Sprite::Release()
{
	SAFE_RELEASE(pTexture_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

void Sprite::InitVertexData()
{
	vertices_ =
	{
		{ XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0, 0.0, 0.0, 0.0)}, 
		{ XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(1.0, 0.0, 0.0, 0.0)},
		{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),  XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) },
		{ XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) },	

	};
	vertexNum_ = vertices_.size();
}

HRESULT Sprite::CreateVertexBuffer()
{
	HRESULT hr;

	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"頂点バッファの作成に失敗", NULL, MB_OK);
		return hr;
	}

	return S_OK;
}

void Sprite::InitIndexData()
{
	indices_ = { 0,2,3, 0,1,2 };
	indexNum_ = indices_.size();
}

HRESULT Sprite::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_; 
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices_.data(); 
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"インデックスバッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Sprite::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"コンスタントバッファの作成に失敗", NULL, MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Sprite::LoadTexture(std::string fileName)
{
	namespace fs = std::filesystem;

	pTexture_ = new Texture;
	assert(fs::is_regular_file(fileName));


	HRESULT hr;
	hr = pTexture_->Load(fileName);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"テクスチャの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

void Sprite::PassDataToCB(DirectX::XMMATRIX worldMatrix)
{
	CONSTANT_BUFFER cb;
	cb.matW = XMMatrixTranspose(worldMatrix);

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);

}

void Sprite::SetBufferToPipeline()
{
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);


}
