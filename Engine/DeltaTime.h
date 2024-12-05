#pragma once
namespace DeltaTime
{
	void Initialize();
	void Update();
	/// <summary>
	/// フレーム間の時間を返す関数
	/// </summary>
	/// フレーム間の時間を秒単位で返す
	/// <returns></returns>
	double GetDeltaTime();
};

