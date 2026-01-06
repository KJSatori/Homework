#include "WaveController.h"
#include "../../Engine/Core/Component/Debug.h"
#include "../../Engine/Core/Prefab/PrefabManager.h"
#include <unordered_map>

struct ScheduledSpawn
{
    float time;
    SpawnInfo info;
};

void WaveController::TriggerSpawn(const SpawnInfo &s)
{
    if (!gameObject) return;
    Scene* scene = gameObject->GetScene();
    if (!scene)
    {
        Debug::Log("[WaveController] TriggerSpawn: scene is null");
        return;
    }
    scene->InstantiatePrefab(s.prefabKey, s.position, s.init);
    // Debug::Log("[WaveController] Spawned " + s.prefabKey + " at" + string(s.position));
}

void WaveController::Start()
{
    elapsed = 0.0f;
    for (auto &w : waves) w.triggered = false;
}

static std::unordered_map<GameObject*, std::vector<ScheduledSpawn>> g_scheduledMap;

void WaveController::Update(float deltaTime)
{
    elapsed += deltaTime;

    // 触发波次
    for (auto &wave : waves)
    {
        if (wave.triggered) continue;
        if (elapsed + 1e-6f >= wave.startTime)
        {
            // 将 SpawnInfo 展开为 ScheduledSpawn
            for (const auto& spawn : wave.spawns)
            {
                for (int i = 0; i < spawn.count; ++i)
                {
                    ScheduledSpawn scheduledSp;
                    scheduledSp.time = wave.startTime + spawn.spawnTime + i * spawn.interval;
                    scheduledSp.info = spawn;
                    g_scheduledMap[gameObject].push_back(scheduledSp);
                }
            }
            wave.triggered = true;
        }
    }

    auto it = g_scheduledMap.find(gameObject);
    if (it != g_scheduledMap.end())
    {
        auto &vec = it->second;
        std::vector<ScheduledSpawn> remaining;
        remaining.reserve(vec.size());
        for (const auto &ss : vec)
        {
            if (elapsed + 1e-6f >= ss.time) TriggerSpawn(ss.info);
            else remaining.push_back(ss);
        }
        vec.swap(remaining);
        if (vec.empty()) g_scheduledMap.erase(it);
    }
}

void WaveController::AddWave(const Wave &w)
{
    waves.push_back(w);
}

void WaveController::Clear()
{
    waves.clear();
    elapsed = 0.0f;
    if (gameObject)
    {
        g_scheduledMap.erase(gameObject);
    }
}
