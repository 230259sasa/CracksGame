#include "Sound.h"
#include<vector>
#include<filesystem>
#include <fstream>
#include <Windows.h>
#include <dsound.h>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

namespace Sound {
	std::vector<SoundData*> soundList;
    LPDIRECTSOUND8 g_pDirectSound = nullptr;
}

int Sound::Load(std::string filename)
{
	SoundData* pData = new SoundData;
	pData->filename_ = filename;

	for (auto& e : soundList)
	{
		//読み込み済みなら
		if (e->filename_ == filename) {
			return e->number_;
		}
	}
	//同名のファイルがなかったら

    
    // DirectSound の初期化（1回のみ）
    if (!g_pDirectSound) {
        if (FAILED(DirectSoundCreate8(NULL, &g_pDirectSound, NULL))) {
            delete pData;
            return -1;
        }
        g_pDirectSound->SetCooperativeLevel(GetForegroundWindow(), DSSCL_PRIORITY);
    }

    // WAVファイルをバイナリモードで読み込む
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        delete pData;
        return -1;
    }

    // WAVヘッダーの解析
    char chunkId[4];
    DWORD chunkSize;
    char format[4];

    file.read(chunkId, 4); // "RIFF"
    file.read(reinterpret_cast<char*>(&chunkSize), 4);
    file.read(format, 4);  // "WAVE"

    if (std::strncmp(chunkId, "RIFF", 4) != 0 || std::strncmp(format, "WAVE", 4) != 0) {
        file.close();
        delete pData;
        return -1; // WAV形式でない
    }

    // フォーマットチャンク
    WAVEFORMATEX waveFormat = {};
    while (file.read(chunkId, 4)) {
        file.read(reinterpret_cast<char*>(&chunkSize), 4);

        if (std::strncmp(chunkId, "fmt ", 4) == 0) {
            file.read(reinterpret_cast<char*>(&waveFormat), sizeof(WAVEFORMATEX));
            file.seekg(chunkSize - sizeof(WAVEFORMATEX), std::ios::cur); // 残りスキップ
        }
        else if (std::strncmp(chunkId, "data", 4) == 0) {
            // サウンドデータの読み込み
            BYTE* waveData = new BYTE[chunkSize];
            file.read(reinterpret_cast<char*>(waveData), chunkSize);

            // バッファ作成
            DSBUFFERDESC bufferDesc = {};
            bufferDesc.dwSize = sizeof(DSBUFFERDESC);
            bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;
            bufferDesc.dwBufferBytes = chunkSize;
            bufferDesc.lpwfxFormat = &waveFormat;

            if (FAILED(g_pDirectSound->CreateSoundBuffer(&bufferDesc, &pData->buffer_, NULL))) {
                delete[] waveData;
                file.close();
                delete pData;
                return -1;
            }

            // バッファにデータコピー
            VOID* pLockedBuffer = nullptr;
            DWORD dwBufferSize = 0;
            if (SUCCEEDED(pData->buffer_->Lock(0, chunkSize, &pLockedBuffer, &dwBufferSize, NULL, NULL, 0))) {
                memcpy(pLockedBuffer, waveData, chunkSize);
                pData->buffer_->Unlock(pLockedBuffer, dwBufferSize, NULL, 0);
            }

            delete[] waveData;
            break;
        }
        else {
            // 不要なチャンクはスキップ
            file.seekg(chunkSize, std::ios::cur);
        }
    }

    file.close();
	
	pData->number_ = soundList.size();
	soundList.push_back(pData);

	return pData->number_;
}

void Sound::Release()
{
}

void Sound::Play(int index, bool isLoop)
{
    soundList[index]->buffer_->SetCurrentPosition(0);
    soundList[index]->buffer_->Play(0, 0, isLoop ? DSBPLAY_LOOPING : 0);
}

void Sound::Stop(int index)
{
    soundList[index]->buffer_->Stop();
}

void Sound::SetVolume(int index, long volume)
{
    soundList[index]->buffer_->SetVolume(volume);
}
