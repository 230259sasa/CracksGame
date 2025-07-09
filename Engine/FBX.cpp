#include "FBX.h"
#include"Camera.h"
#include<filesystem>
#include<DirectXCollision.h>
#include "FbxParts.h"

namespace fs = std::filesystem;

FBX::FBX()
	:pFbxManager_(nullptr),pFbxScene_(nullptr),shaderType_(SHADER_3D)
{
}

//ロードしていろいろ初期化
HRESULT FBX::Load(std::string fileName)
{
	//マネージャを生成
	pFbxManager_ = FbxManager::Create();

	//インポーターを生成
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager_, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager_->GetIOSettings());

	//シーンオブジェクトにFBXファイルの情報を流し込む
	pFbxScene_ = FbxScene::Create(pFbxManager_, "fbxscene");
	fbxImporter->Import(pFbxScene_);
	fbxImporter->Destroy();

	FbxGeometryConverter geometryConverter(pFbxManager_);
	geometryConverter.Triangulate(pFbxScene_, true);

	fs::path cPath, basePath;
	cPath = fs::current_path();
	basePath = cPath;
	string subDir("Assets");
	//Assetsフォルダまでのフルパスを生成する
	fs::path subPath(cPath.string() + "\\" + subDir);
	assert(fs::exists(subPath));
	//カレントディレクトリを移動
	fs::current_path(subPath);

	int meshCount = pFbxScene_->GetSrcObjectCount<FbxMesh>();
	for (int i = 0; i < meshCount; ++i)
	{
		// <たったこれだけで全てのメッシュデータを取得できる>
		FbxMesh* mesh = pFbxScene_->GetSrcObject<FbxMesh>(i);
		//パーツを用意
		FbxParts* pParts = new FbxParts(this);
		pParts->Init(mesh);

		//パーツ情報を動的配列に追加
		parts_.push_back(pParts);

	}

	//カレントパスをもとどおりにもどす
	fs::current_path(basePath);

	//マネージャ解放
	//pFbxManager_->Destroy();
	return S_OK;
}

void FBX::Draw(Transform& transform, int frame)
{
	Direct3D::SetBlend(BLEND_INVALID);
	Direct3D::SetShader(shaderType_);

	//パーツを1個ずつ描画
	for (int k = 0; k < parts_.size(); k++)
	{
		// その瞬間の自分の姿勢行列を得る
		FbxTime     time;
		time.SetTime(0, 0, 0, frame, 0, 0, frameRate_);

		//スキンアニメーション（ボーン有り）の場合
		if (parts_[k]->GetSkinInfo() != nullptr)
		{
			parts_[k]->DrawSkinAnime(transform, time);
		}

		//メッシュアニメーションの場合
		else
		{
			parts_[k]->DrawMeshAnime(transform, time, pFbxScene_);
		}
	}
}

void FBX::OutLineDraw(Transform& transform)
{
	Direct3D::SetShader(SHADER_OUTLINE);
	Draw(transform,0);
}

void FBX::Release()
{
}

void FBX::RayCast(RayCastData& rayData, Transform& transform)
{
	rayData.dist = 999.0f;
	for (int i = 0; i < parts_.size(); i++)
	{
		parts_[i]->RayCast(rayData,transform);
	}
}

bool FBX::GetBonePositionAtNow(std::string boneName, XMFLOAT3* position)
{
	for (int i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetBonePositionAtNow(boneName, position)) {
			return true;
		}
	}
	return false;
}

void FBX::SetShaderType(SHADER_TYPE type)
{
	shaderType_ = type;
}
