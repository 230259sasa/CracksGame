#pragma once
#include<string>
#include"FBX.h"

namespace Model 
{
	struct ModelData {
		FBX* pFbx_;
		Transform transform_;
		std::string filename_;
		//アニメーションのフレーム
		float nowFrame, animSpeed;
		int startFrame, endFrame;

		ModelData()
			:pFbx_(nullptr), filename_(""),transform_(),nowFrame(0),
		animSpeed(0),startFrame(0),endFrame(0){};

		/// <summary>
		/// アニメーションのフレーム数をセット
		/// </summary>
		/// <param name="start">開始フレーム</param>
		/// <param name="end">終了フレーム</param>
		/// <param name="speed">アニメーション速度</param>
		void SetAnimFrame(int start, int end, float speed)
		{
			nowFrame = (float)start;
			startFrame = start;
			endFrame = end;
			animSpeed = speed;
		}
	};

	int Load(std::string filename);
	void SetTransform(int hModel, Transform transform);
	void SetAnimFrame(int hModel, int startFrame, int endFrame, float animSpeed);
	void SetAnimFrame(int hModel, int startFrame, int endFrame);
	void SetShaderType(int hModel, SHADER_TYPE type);
	int GetAnimFrame(int hModel);
	bool GetBonePositionAtNow(int hModel,std::string boneName, XMFLOAT3* position);
	void Draw(int hModel);
	void OutLineDraw(int hModel);
	void Release();
	void RayCast(int hModel, RayCastData& rayData, Transform& transform);
}