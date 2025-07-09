#pragma once
#include "Engine\GameObject.h"
#include "Engine\FBX.h"

class Sprite;
class FallObject;

class FallBlockManager :
	public GameObject
{
    float time_;
    enum FallObjectType {
        BLOCK,
        BOMB,
        MAX
    };
    std::vector<XMINT3> fallPos_;
public:
    FallBlockManager(GameObject* parent);
    ~FallBlockManager();
    void Initialize() override;
    void Release() override;
    void Update() override;
    void Draw() override;
    int GetOnGroundBlockNum();
    std::vector<XMFLOAT4> GetFallingObjectCenterPosition();
private:
    void FallControle();
};

