#pragma once
#include<string>
#include <dsound.h>

#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )

namespace Sound {
	struct SoundData {
		int number_;
		std::string filename_;
		LPDIRECTSOUNDBUFFER buffer_;
		SoundData():
		number_(0),filename_(""),buffer_(nullptr){};
	};
	void Initialize();
	int Load(std::string filename);
	void Release();
	void Play(int index,bool isLoop);
	void Stop(int index);
	void SetVolume(int index, long volume);
}

