#pragma once
#include<string>
#include"FBX.h"

namespace Model 
{
	struct ModelData {
		FBX* pFbx_;
		Transform transform_;
		std::string filename_;
		ModelData()
			:pFbx_(nullptr), filename_(""),transform_() {};
	};
	//model��ۑ�����z��
	int Load(std::string filename);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);
	void OutLineDraw(int hModel);
	void Release();
	void RayCast(int hModel, RayCastData& rayData, Transform& transform);
}