#ifndef STAGELOADER_H
#define STAGELOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
using json = nlohmann::json;

#include "WaveController.h"
#include "EnemyBezierSniper.h"
#include "EnemyBezierFixer.h"
#include "BulletConfig.h"
#include "../../Engine/Math/Utils.h"

static Vector2 VecFromJson(const json& a)
{
    Debug::Log("Test1");
    if (!a.is_array() || a.size() < 2) return Vector2::Zero();
    Debug::Log("Test2");
    return Vector2(a[0].get<float>(), a[1].get<float>());
}

static BulletConfig BulletConfigFromJson(const json& j, bool isSniper)
{
    // std::string sprite = j.value("sprite", std::string("PurpleBullet45D"));
    std::string sprite  = (isSniper) ?  std::string("PurpleBullet45D") :  std::string("RedBullet");
    float scale         = j.value("scale", 2.0);
    float colRadius     = (isSniper) ? scale*2.0f : scale*2.5f;
    float speed         = j.value("speed", 50.0f);
    float life          = j.value("life", 1.75f);
    Tag tag             = Tag::EnemyBullet;
    int damage          = 0;
    Vector2 dir         = j.contains("dir") ? VecFromJson(j["dir"]) : Vector2::Right();
    Vector2 spawnOffset = j.contains("spawnOffset") ? VecFromJson(j["spawnOffset"]) : Vector2::Zero(); 

    if (!isSniper)
    {
        float bulletDuration        = j.value("bulletDuration", 3.0f);
        bool bulletApplyEasing      = j.value("bulletApplyEasing", false);
        float bulletEasingTime      = j.value("bulletEasingTime", 2.0f);
        float bulletEasingSlope     = j.value("bulletEasingSlope", 0.5f);
        bool bulletApplyEasingStop  = j.value("bulletApplyEasingStop", false);
        float bulletStopDuration    = j.value("bulletStopDuration", 0.0f);

        std::vector<Vector2> bulletBezier;
        if (j.contains("bulletBezier") && j["bulletBezier"].is_array())
        {
            for (const auto& p : j["bulletBezier"]) bulletBezier.push_back(VecFromJson(p));
        }
        std::function<float(float)> func = [bulletDuration, bulletApplyEasing, bulletEasingTime, bulletEasingSlope, bulletApplyEasingStop, bulletStopDuration] (float age) 
        { 
            if (!bulletApplyEasing) return age;
            if (!bulletApplyEasingStop) return DurationCubeFunc(age, bulletDuration, bulletEasingTime, bulletEasingSlope);
            if (age < bulletEasingTime) return DurationCubeFunc(age, bulletDuration - bulletStopDuration, bulletEasingTime, bulletEasingSlope);
            else if (age >= bulletEasingTime && age < bulletEasingTime + bulletStopDuration) return DurationCubeFunc(bulletEasingTime, bulletDuration - bulletStopDuration, bulletEasingTime, bulletEasingSlope);
            else return DurationCubeFunc(age - bulletStopDuration, bulletDuration - bulletStopDuration, bulletEasingTime, bulletEasingSlope);
        };
        return BulletConfig(sprite, scale, colRadius, tag, speed, damage, life, dir, spawnOffset, make_shared<BezierPattern>(bulletBezier, bulletDuration), func);
    }
    else return BulletConfig(sprite, scale, colRadius, tag, speed, damage, life, dir, spawnOffset);
}

/// @brief 从 Json 文件加载关卡
/// @param path Json 文件路径
/// @param controller 波数控制器脚本
/// @return bool 表示加载成功与否
bool loadStageFromJson(const std::string& path, WaveController& controller)
{
    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        Debug::Log("[StageLoader] open file failed: " + path);
        return false;
    }

    json root;
    try
    {
        ifs >> root;
    }
    catch(const std::exception& e)
    {
        Debug::Log(std::string("[StageLoader] parse json failed: ") + e.what());
        return false;
    }

    if (!root.contains("waves") || !root["waves"].is_array())
    {
        Debug::Log("[StageLoader] json missing waves array");
        return false;
    }
    
    for (const auto& wj : root["waves"])
    {
        Wave w;
        w.startTime = wj.value("startTime", 0.0f);
        if (wj.contains("spawns") && wj["spawns"].is_array())
        {
            for (const auto& sj : wj["spawns"])
            {
                SpawnInfo s;

                s.prefabKey = sj.value("prefabKey", std::string("enemyBezierSniper"));
                s.position  = VecFromJson(sj["position"]);
                s.spawnTime = sj.value("spawnTime", 0.0f);
                s.count     = sj.value("count", 1);
                s.interval  = sj.value("interval", 1.0f);

                std::vector<BulletConfig> bullets;
                if (sj.contains("bulletConfigs") && sj["bulletConfigs"].is_array())
                {
                    for (const auto& bj : sj["bulletConfigs"]) bullets.push_back(BulletConfigFromJson(bj, s.prefabKey == "enemyBezierSniper"));
                }

                std::vector<Vector2> bezier;
                if (sj.contains("bezier") && sj["bezier"].is_array())
                {
                    for (const auto& p : sj["bezier"]) bezier.push_back(VecFromJson(p));
                }

                int   health        = 1;
                float startShooting = 0.0f;
                float endShooting   = 0.0f;
                float fireInterval  = 1.0f;
                float duration      = 4.0f;

                bool  applyEasing   = false;
                float easingTime    = 2.0f;
                float easingSlope   = 0.5f;
                bool applyEasingStop= false;
                float stopDuration  = 0.0f; 

                if (sj.contains("params") && sj["params"].is_object())
                {
                    auto pj         = sj["params"];
                    health          = pj.value("health", health);
                    startShooting   = pj.value("startShooting", startShooting);
                    endShooting     = pj.value("endShooting", endShooting);
                    fireInterval    = pj.value("fireInterval", fireInterval);
                    duration        = pj.value("duration", duration);

                    applyEasing     = pj.value("applyEasing", applyEasing);
                    easingTime      = pj.value("easingTime", easingTime);
                    easingSlope     = pj.value("easingSlope", easingSlope);
                    applyEasingStop = pj.value("applyEasingStop", applyEasingStop);
                    stopDuration    = pj.value("stopDuration", stopDuration);
                }

                if (s.prefabKey == "enemyBezierSniper")
                {
                    s.init = [=](std::shared_ptr<GameObject> obj)
                    {
                        auto script = obj->GetComponent<EnemyBezierSniper>();
                        if (!script) return;
                        std::vector<Vector2> pts = bezier.empty() ? std::vector<Vector2>{Vector2::Zero(), Vector2::Down() * 700.0f} : bezier; 

                        script->SetParameters(health, startShooting, endShooting, fireInterval, duration, bullets, pts, 
                            [duration, applyEasing, easingTime, easingSlope, applyEasingStop, stopDuration] (float age) 
                            { 
                                if (!applyEasing) return age;
                                if (!applyEasingStop) return DurationCubeFunc(age, duration, easingTime, easingSlope);
                                if (age < easingTime) return DurationCubeFunc(age, duration - stopDuration, easingTime, easingSlope);
                                else if (age >= easingTime && age < easingTime + stopDuration) return DurationCubeFunc(easingTime, duration - stopDuration, easingTime, easingSlope);
                                else return DurationCubeFunc(age - stopDuration, duration - stopDuration, easingTime, easingSlope);
                            }
                        );
                    };
                }
                else if (s.prefabKey == "enemyBezierFixer")
                {

                    float bulletDuration = sj.value("bulletDuration", 3.0f);
                    s.init = [=](std::shared_ptr<GameObject> obj)
                    {
                        auto script = obj->GetComponent<EnemyBezierFixer>();
                        if (!script) return;
                        std::vector<Vector2> movePts = bezier.empty() ? std::vector<Vector2>{Vector2::Zero(), Vector2::Down() * 700.0f} : bezier; 

                        script->SetParameters(health, startShooting, endShooting, fireInterval, duration, bullets, movePts,  
                            [duration, applyEasing, easingTime, easingSlope, applyEasingStop, stopDuration] (float age) 
                            { 
                                if (!applyEasing) return age;
                                if (!applyEasingStop) return DurationCubeFunc(age, duration, easingTime, easingSlope);
                                if (age < easingTime) return DurationCubeFunc(age, duration - stopDuration, easingTime, easingSlope);
                                else if (age >= easingTime && age < easingTime + stopDuration) return DurationCubeFunc(easingTime, duration - stopDuration, easingTime, easingSlope);
                                else return DurationCubeFunc(age - stopDuration, duration - stopDuration, easingTime, easingSlope);
                            }
                        );
                    };
                }
                else
                {
                    s.init = nullptr;
                }

                w.spawns.push_back(s);
            }
        }
        controller.AddWave(w);
    }
    Debug::Log("[StageLoader] loaded stage from " + path); 
    return true;
}

void GenerateRandomStage(WaveController& controller, int seed = 0)
{
    // 哎, C++想生成个随机数太麻烦了, 这么一大坨
    // 你跟我说 rand()? 这个并不是 "Modern C++" 提倡的啊!
    auto srandSeed = seed ? seed : (int)std::chrono::steady_clock::now().time_since_epoch().count();
    std::mt19937 rng(srandSeed);
    std::uniform_real_distribution<float> xdist(0.0f, 560.0f);
    std::uniform_real_distribution<float> xInBorderDist(40.0f, 520.0f);
    std::uniform_real_distribution<float> rightXdist(280.0f, 560.0f);
    std::uniform_real_distribution<float> leftXdist(0.0f, 280.0f);
    std::uniform_real_distribution<float> ydist(400.0f, 620.0f);
    std::uniform_real_distribution<float> yUpper(600.0f, 610.0f);
    std::uniform_real_distribution<float> yBottom(0.0f, 10.0f);
    std::uniform_real_distribution<float> yDowndist(0.0f, 200.0f);
    std::uniform_real_distribution<float> tdist(0.0f, 2.0f);
    std::uniform_real_distribution<float> unitDist(-0.5f, 0.5f);
    std::uniform_real_distribution<float> speedDist(25.0f, 900.0f);
    std::uniform_real_distribution<float> durationDist(2.0f, 7.5f);
    std::uniform_real_distribution<float> fireInterval(0.1f, 2.5f);
    std::uniform_real_distribution<float> startShootingdist(0.1f, 1.0f);
    std::uniform_real_distribution<float> endShootingdist(5.0f, 15.0f);
    std::uniform_real_distribution<float> moveDurationdist(1.0f, 10.0f);
    std::uniform_int_distribution<int> typeDist(0, 1);
    std::uniform_int_distribution<int> countDist(1, 8);
    std::uniform_real_distribution<float> intervalDist(0.2f, 1.0f);
    std::uniform_real_distribution<float> cpDist(-700.0f, -100.0f);

    int waveCount = 999; // 999 waves 我就不信你打的完
    float timeCursor = 0.0f;
    for (int wi = 0; wi < waveCount; ++wi)
    {
        Wave w;
        w.startTime = timeCursor;
        int spawns = wi / 2 + 1;
        for (int si = 0; si < spawns; ++si)
        {
            SpawnInfo s;
            bool isFixer = (typeDist(rng) == 1);
            s.prefabKey = isFixer ? "enemyBezierFixer" : "enemyBezierSniper";
            float spawnX = xdist(rng);
            float spawnY = (spawnX < 40.0f || spawnX > 520.f) ? ydist(rng) : yUpper(rng);
            s.position  = Vector2(spawnX, spawnY);
            s.spawnTime = tdist(rng);
            s.count     = countDist(rng);
            s.interval  = intervalDist(rng);

            // random 贝塞尔曲线点
            std::vector<Vector2> bezier;
            float endOfBezierX = (spawnX > 280.0f) ? leftXdist(rng) : rightXdist(rng);
            float endOfBezierY = (endOfBezierX < 30.0f || endOfBezierX > 530.0f) ? yDowndist(rng) : yBottom(rng);
            Vector2 endOfBezier = Vector2(endOfBezierX, endOfBezierY) - s.position;
            int numOfBezierPoint = countDist(rng);
            bezier.push_back(Vector2(0.0f, 0.0f)); 
            
            for (int i = 0; i < numOfBezierPoint; ++i)
            {
                bezier.push_back(Vector2((float)(xInBorderDist(rng) - 280.0f) * float(i) / float(countDist(rng)), cpDist(rng)* float(i) / float(countDist(rng)))); 
            }
            bezier.push_back(endOfBezier);

            float purpleScale = 2.0f + unitDist(rng);
            BulletConfig purpleCfg("PurpleBullet45D", purpleScale, purpleScale*2.0f, Tag::EnemyBullet, speedDist(rng), 0, 7.5f, Vector2::Right(), Vector2::Right() * 5.0f); 
            std::vector<BulletConfig> purpleBullets{purpleCfg};

            float redScale = 2.0f + unitDist(rng);
            BulletConfig redCfg("RedBullet", redScale, redScale * 2.5f, Tag::EnemyBullet, speedDist(rng), 0, 7.5f, Vector2::Down(), Vector2::Down() * 5.0f, make_shared<BezierPattern>(bezier, durationDist(rng)), [](float age) { return age; }); 
            std::vector<BulletConfig> redBullets{redCfg};

            // 注意！这里所有随机函数都是我瞎整的。勿动！
            int health = countDist(rng) % 3;
            float fireItv = fireInterval(rng);
            float moveDuration = moveDurationdist(rng);
            float stShooting = startShootingdist(rng);
            float edShooting = endShootingdist(rng);
            if (!isFixer) 
            { 
                s.init = [=](std::shared_ptr<GameObject> obj) 
                { 
                    auto script = obj->GetComponent<EnemyBezierSniper>(); 
                    if (!script) return; 
                    // fxxk 仅支持线性easing
                    script->SetParameters(health, stShooting, edShooting, fireItv, moveDuration, purpleBullets, bezier, [] (float a) { return a; }); 
                }; 
            }
            else 
            { 
                s.init = [=](std::shared_ptr<GameObject> obj) 
                { 
                    auto script = obj->GetComponent<EnemyBezierFixer>(); 
                    if (!script) return; 
                    script->SetParameters(health, stShooting, edShooting, fireItv, moveDuration, redBullets, bezier, [] (float a) { return a; }); 
                }; 
            }
            w.spawns.push_back(s);
        }
        controller.AddWave(w);
        timeCursor += 4.0f + (rng()%5);
    }
    Debug::Log(string("[StageLoader] generated random stage. Seed is ") + to_string(srandSeed));
}

#endif