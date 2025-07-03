#pragma once
#include "Direct3D.h"
#include<fbxsdk.h>
#include <DirectXMath.h>
#include "Texture.h"
#include<vector>
#include"Transform.h"
#include<filesystem>

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

//レイキャスト用構造体
struct RayCastData
{
	XMFLOAT4	start;	//Rayの位置
	XMFLOAT4	dir;	//Rayのベクトル
	float       dist;	//衝突までの距離
	bool		hit;	//Rayが当たったか
};

class FbxParts;

class FBX
{
	friend class FbxParts;
	std::vector<FbxParts*>	parts_;

	FbxManager* pFbxManager_;
	FbxScene* pFbxScene_;
	FbxTime::EMode	frameRate_;
public:
	FBX();
	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform,int frame);
	void    OutLineDraw(Transform& transform);
	void    Release();
	void	RayCast(RayCastData& rayData, Transform& transform);
	bool	GetBonePosition(std::string boneName, XMFLOAT3* position);
};
