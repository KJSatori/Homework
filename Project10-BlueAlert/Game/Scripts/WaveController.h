#ifndef WAVECONTROLLER_H
#define WAVECONTROLLER_H

#include <vector>
#include <functional>
#include <memory>
#include <string>
#include "../../Engine/Math/Vector2.h"
#include "../../Engine/Core/GameObject/GameObject.h"
#include "../../Engine/Core/Component/Script.h"
#include "../../Engine/Core/Scene/Scene.h"

struct SpawnInfo
{
    std::string prefabKey;
    Vector2 position;
    float spawnTime = 0.0f;
    int count = 1;
    float interval = 0.0f;
    std::function<void(std::shared_ptr<GameObject>)> init;
};

struct Wave
{
    float startTime = 0.0f;
    std::vector<SpawnInfo> spawns;
    bool triggered = false;
};

class WaveController : public Script
{
private:
    float elapsed = 0.0f;
    std::vector<Wave> waves;
    void TriggerSpawn(const SpawnInfo& s);
public:
    WaveController(GameObject* owner) : Script(owner) {}
    void Start() override;
    void Update(float deltaTime) override;
    void AddWave(const Wave& w);
    void Clear();
};

#endif