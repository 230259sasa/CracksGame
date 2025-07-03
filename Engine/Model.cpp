#include"Model.h"
#include"Direct3D.h"

namespace Model {
	//modelを保存する配列
	std::vector<ModelData*> modelList;
}

int Model::Load(std::string filename)
{
	//読み込み
	//既に読み込まれていたら -1
	//初めてなら読み込んで配列に追加
	ModelData* pData = new ModelData;
	pData->filename_ = filename;
	pData->pFbx_ = nullptr;

	for (auto& e : modelList)
	{
		//読み込み済みなら
		if (e->filename_ == filename) {
			pData->pFbx_ = e->pFbx_;
			break;
		}
	}
	//同名のファイルがなかったら
	if (pData->pFbx_ == nullptr) {
		pData->pFbx_ = new FBX;
		pData->pFbx_->Load("Assets/Model/" + filename);
	}
	modelList.push_back(pData);
	
	return(modelList.size()-1);
}

void Model::SetTransform(int hModel, Transform transform)
{
	modelList[hModel]->transform_ = transform;
}

void Model::SetAnimFrame(int hModel, int startFrame, int endFrame, float animSpeed)
{
	modelList[hModel]->SetAnimFrame(startFrame, endFrame, animSpeed);
}

void Model::SetAnimFrame(int hModel, int startFrame, int endFrame)
{
	SetAnimFrame(hModel, startFrame, endFrame, 1.0f);
}

int Model::GetAnimFrame(int hModel)
{
	return modelList[hModel]->nowFrame;
}

bool Model::GetBonePosition(int hModel, std::string boneName, XMFLOAT3* position)
{
	return modelList[hModel]->pFbx_->GetBonePosition(boneName, position);
}

void Model::Draw(int hModel)
{
	if (hModel < 0 || hModel >= modelList.size() || modelList[hModel] == nullptr)
	{
		return;
	}

	//アニメーションを進める
	modelList[hModel]->nowFrame += modelList[hModel]->animSpeed;

	//最後までアニメーションしたら戻す
	if (modelList[hModel]->nowFrame > (float)modelList[hModel]->endFrame)
		modelList[hModel]->nowFrame = (float)modelList[hModel]->startFrame;



	if (modelList[hModel]->pFbx_)
	{
		modelList[hModel]->pFbx_->Draw(modelList[hModel]->transform_, (int)modelList[hModel]->nowFrame);
	}

	//modelList[hModel]->pFbx_->ThreeDimensionalDraw(modelList[hModel]->transform_);
}

void Model::OutLineDraw(int hModel)
{
	modelList[hModel]->pFbx_->OutLineDraw(modelList[hModel]->transform_);
}

void Model::Release()
{
	bool isReffered = false;
	for (int j = 0; j < modelList.size(); j++) {
		for (int i = j+1; i < modelList.size(); i++) {
			//同じモデルなら
			if (modelList[j]->pFbx_ == modelList[i]->pFbx_) {
				isReffered = true;
				break;
			}
		}
		if (!isReffered) {
			SAFE_DELETE(modelList[j]->pFbx_);
		}
		SAFE_DELETE(modelList[j]);
	}
	modelList.clear();
}

void Model::RayCast(int hModel, RayCastData& rayData, Transform& transform)
{
	modelList[hModel]->pFbx_->RayCast(rayData, transform);
}
