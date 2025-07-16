#pragma once
#include "Engine\GameObject.h"
#include "Engine\FBX.h"

class Timer;
class Sprite;
class FallObject;

class FallBlockManager :
	public GameObject
{
    Timer* timer_;
    enum FallObjectType {
        BLOCK,
        BOMB,
        MAX
    };
    std::vector<XMINT3> fallPos_;
    bool isFirstBlock_;
public:
    FallBlockManager(GameObject* parent);
    ~FallBlockManager();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
    void StopSpawnBlock();
    void StartSpawnBlock();
    int GetOnGroundBlockNum();
    std::vector<XMFLOAT4> GetFallingObjectCenterPosition();
private:
    void FallControle();
};

