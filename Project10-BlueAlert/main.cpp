#define _XOPEN_SOURCE_EXTENDED 1
#include <locale.h>
#include <ncursesw/ncurses.h>
#include <functional>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Engine/Utils/CharWidth.h"

#include "Engine/Math/Vector2.h"
#include "Engine/Math/Transform.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Core/GameObject/GameObject.h"
#include "Engine/Core/Component/RendererComponent.h"
#include "Engine/Core/Component/Rigidbody.h"
#include "Engine/Core/Component/Collider.h"
#include "Engine/Core/Component/InputHandler.h"
#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/Component/PlayerController.h"

#include "Game/Prefabs/RegisterPrefabs.h"
#include "Game/Scripts/EnemyBezierSniper.h"
#include "Game/Scripts/EnemyBezierFixer.h"
#include "Game/Scripts/WaveController.h"
#include "Game/Scripts/StageLoader.h"

using Clock = std::chrono::steady_clock;

inline std::function<void(std::shared_ptr<GameObject>)> MakeBezierSniperEnemyInit
(
    int health, float startShooting, float endShooting, float fireInterval,float duration,
    const std::vector<BulletConfig>& bullets, const std::vector<Vector2>& bezierPoints,
    std::function<float(float)> easing
)
{
    return [=](std::shared_ptr<GameObject> obj) 
    { 
        auto enemyScript = obj->GetComponent<EnemyBezierSniper>(); 
        if (!enemyScript) return; 
        enemyScript->SetParameters(health, startShooting, endShooting, fireInterval, duration, bullets, bezierPoints, easing); 
    };
}

inline std::function<void(std::shared_ptr<GameObject>)> MakeBezierFixedEnemyInit
(
    int health, float startShooting, float endShooting, float fireInterval, float moveDuration,
    const std::vector<BulletConfig>& bulletConfigs, const std::vector<Vector2>& moveCtrlPoints,
    float bulletDuration, const std::vector<Vector2>& bulletCtrlPoints,
    const std::function<float(float)>& LinearToEasing, const std::function<float(float)>& BulletEasing
)
{
    return [=](std::shared_ptr<GameObject> obj)
    {
        auto enemyScript = obj->GetComponent<EnemyBezierFixer>();
        if (!enemyScript) return;
        enemyScript->SetParameters
        (
            health, startShooting, endShooting, fireInterval, moveDuration, 
            bulletConfigs, moveCtrlPoints,
            LinearToEasing
        );
    };
}

void InitWave1(Wave& wave)
{
    BulletConfig publicConfig = BulletConfig("PurpleBullet45D",
        2.0f, 1.75f * 2.0f, Tag::EnemyBullet, 600.0f,
        0, 3.0f, Vector2(1, 0), Vector2(1, 0) * 5.0f
    );

    /*// 贝塞尔曲线子弹不需要dir
    BulletConfig config2 = BulletConfig("RedBullet",
        2.0f, 1.0f * 2.0f, Tag::EnemyBullet, 100.0f,
        0, 10.0f, Vector2::Zero(), Vector2::Zero()
    );

    SpawnInfo testS;
    testS.prefabKey = "enemyBezierFixer";
    testS.position = Vector2(280, 600);
    testS.spawnTime = 0.0f;
    testS.count = 2;
    testS.interval = 1.0f;
    testS.init = MakeBezierFixedEnemyInit
    (
        2, 0.0f, 10.0f, 1.0f, 10.0f, {config2},
        {Vector2::Zero(), Vector2::Down() * 700.0f},
        4.0f, {Vector2::Zero(), Vector2::Down() * 700.0f},
        [](float age){return age;},
        [](float age){return age;}
    );
    wave.spawns.push_back(testS);*/
    struct Row { float x, y, t, cp3y; }; 
    Row rows[] = { { 0.0f, 610.0f, 0.0f, -300.0f }, { -10.0f, 580.0f, 0.3f, -280.0f }, { -20.0f, 550.0f, 0.6f, -260.0f } };

    for (auto &r : rows)
    {
        SpawnInfo s;
        s.prefabKey = "enemyBezierSniper";
        s.position = Vector2(r.x, r.y);
        s.spawnTime = r.t;
        s.count = 8;
        s.interval = 0.4f;
        
        std::vector<Vector2> bezier = 
        {
            Vector2(0.0f, 0.0f),
            Vector2(250.0f, -250.0f),
            Vector2(610.0f, r.cp3y)
        };
        s.init = MakeBezierSniperEnemyInit(1, 1.5, 2.5f, 2.0f, 4.0f,
            std::vector<BulletConfig>{publicConfig}, bezier,
            [] (float age) {return DurationCubeFunc(age, 4, 2, 0.3333); /*((age-2)*(age-2)*(age-2)+2*age)/6 + 1.26f;*/ }
        );

        wave.spawns.push_back(s);
    }
}

void InitWave2(Wave& wave)
{
    BulletConfig publicConfig = BulletConfig("PurpleBullet45D",
        2.0f, 1.75f * 2.0f, Tag::EnemyBullet, 600.0f,
        0, 3.0f, Vector2(1, 0), Vector2(1, 0)
    );

    struct Row { float x, y, t, cp3y; }; 
    Row rows[] = { { 560.0f, 610.0f, 0.0f, -300.0f }, { 570.0f, 580.0f, 0.3f, -280.0f }, { 580.0f, 550.0f, 0.6f, -260.0f }};

    for (auto &r : rows)
    {
        SpawnInfo s;
        s.prefabKey = "enemyBezierSniper";
        s.position = Vector2(r.x, r.y);
        s.spawnTime = r.t;
        s.count = 8;
        s.interval = 0.4f;
        
        std::vector<Vector2> bezier = 
        {
            Vector2(0.0f, 0.0f),
            Vector2(-250.0f, -250.0f),
            Vector2(-610.0f, r.cp3y)
        };
        s.init = MakeBezierSniperEnemyInit(1, 1.5, 2.5f, 2.0f, 4.0f,
            std::vector<BulletConfig>{publicConfig}, bezier,
            [] (float age) {return ((age-2)*(age-2)*(age-2)+2*age)/6 + 1.26f; }
        );

        wave.spawns.push_back(s);
    }
}

void InitWave3(Wave& wave)
{
    BulletConfig publicConfig = BulletConfig("PurpleBullet45D",
        2.0f, 1.75f * 2.0f, Tag::EnemyBullet, 600.0f,
        0, 3.0f, Vector2(1, 0), Vector2(1, 0)
    );

    struct Row { float x, y, t, cp3y; }; 
    Row rows1[] = { { 0.0f, 610.0f, 0.0f, -300.0f }, { -10.0f, 580.0f, 0.3f, -280.0f }, { -20.0f, 550.0f, 0.6f, -260.0f } };
    Row rows2[] = { { 560.0f, 610.0f, 0.0f, -300.0f }, { 570.0f, 580.0f, 0.3f, -280.0f }, { 580.0f, 550.0f, 0.6f, -260.0f } };

    for (auto &r : rows1)
    {
        SpawnInfo s;
        s.prefabKey = "enemyBezierSniper";
        s.position = Vector2(r.x, r.y);
        s.spawnTime = r.t;
        s.count = 8;
        s.interval = 1.0f;
        
        std::vector<Vector2> bezier = 
        {
            Vector2(0.0f, 0.0f),
            Vector2(250.0f, -250.0f),
            Vector2(610.0f, r.cp3y)
        };
        s.init = MakeBezierSniperEnemyInit(1, 1.5, 2.5f, 2.0f, 4.0f,
            std::vector<BulletConfig>{publicConfig}, bezier,
            [] (float age) {return ((age-2)*(age-2)*(age-2)+2*age)/6 + 1.26f; }
        );

        wave.spawns.push_back(s);
    }

    for (auto &r : rows2)
    {
        SpawnInfo s;
        s.prefabKey = "enemyBezierSniper";
        s.position = Vector2(r.x, r.y);
        s.spawnTime = r.t;
        s.count = 8;
        s.interval = 1.0f;
        
        std::vector<Vector2> bezier = 
        {
            Vector2(0.0f, 0.0f),
            Vector2(-250.0f, -250.0f),
            Vector2(-610.0f, r.cp3y)
        };
        s.init = MakeBezierSniperEnemyInit(1, 1.5, 2.5f, 2.0f, 4.0f,
            std::vector<BulletConfig>{publicConfig}, bezier,
            [] (float age) {return ((age-2)*(age-2)*(age-2)+2*age)/6 + 1.26f; }
        );

        wave.spawns.push_back(s);
    }
}

void InitWave4(Wave& wave)
{
    BulletConfig dropConfig1 = BulletConfig("RedBullet",
        2.0f, 1.0f*2.0f, Tag::EnemyBullet, 100.0f,
        0, 3.0f, Vector2::Zero(), Vector2(-6.0f, 5.0f)
    );
    BulletConfig dropConfig2 = BulletConfig("RedBullet",
        2.0f, 1.0f*2.0f, Tag::EnemyBullet, 100.0f,
        0, 3.0f, Vector2::Zero(), Vector2(6.0f, 5.0f)
    );

    struct Row { float x, t; };
    Row rows1[] = { {448.0f, 0.0f}, {112.0f, 1.0f}, {280.0f, 2.0f} };

    for (auto& r : rows1)
    {
        SpawnInfo enemy;
        enemy.prefabKey = "enemyBezierFixer";
        enemy.position = Vector2(r.x, 600.0f);
        enemy.spawnTime = r.t,
        enemy.count = 1;
        enemy.interval = 1.0f;
        enemy.init = MakeBezierFixedEnemyInit
        (
            8, 2.0f, 5.0f, 0.33f, 8.0f, {dropConfig1, dropConfig2},
            {Vector2(0, 0), Vector2(0, -660.0f)},
            3.0f, {Vector2(0, 0), Vector2(0, -660.0f)},
            [] (float age) 
            {
                if (age < 2.0f) return DurationCubeFunc(age, 5.0f, 2.0f, 0.0f); 
                else if (age >= 2.0f && age < 5.0f) return DurationCubeFunc(2.0f, 5.0f, 2.0f, 0.0f);
                else return DurationCubeFunc(age - 3.0f, 5.0f, 2.0f, 0.0f);
            },
            [] (float age) {return age; }
        );
        wave.spawns.push_back(enemy);
    }


    Row rows2[] = { {200.0f, 0.5f}, {360.0f, 1.5f} };
    for (auto& r : rows2)
    {
        SpawnInfo ghost;
        ghost.prefabKey = "enemyBezierSniper";
        ghost.position = Vector2(r.x, 600.0f);
        ghost.spawnTime = r.t;
        ghost.interval = 0.5f;
        ghost.count = 8;

        std::vector<Vector2> bezier =
        {
            Vector2(0.0f, 0.0f), Vector2(-75.0f, -100.0f),
            Vector2(75.0f, -200.0f), Vector2(-75.0f, -300.0f),
            Vector2(75.0f, -400.0f), Vector2(-75.0f, -500.0f),
            Vector2(75.0f, -600.0f), Vector2(-75.0f, -700.0f)
        };

        ghost.init = MakeBezierSniperEnemyInit(4, 0.0f, 0.0f, 1.0f, 5.0f, {}, bezier, [] (float age) { return age; });
        wave.spawns.push_back(ghost);
    }
}

int main(int argc, char** argv) {
    auto last = Clock::now();
    const int TARGET_FPS = 180;
    const float TARGET_FRAME = 1.0f / TARGET_FPS;
    const float MAX_DT = 0.05f;

    // 每帧 tick++ 65535回滚至0 一般用于调试
    int tick = 0;

    // 初始化 SDL
    Renderer renderer(1280, 960); 
    int w, h;
    SDL_GetWindowSize(renderer.GetWindow(), &w, &h);
    renderer.UpdateViewport(w, h);

    // 加载资源
    renderer.LoadTexture("YellowPlayerShip", "./Assets/YellowPlayerShip.png");
    renderer.LoadTexture("Border", "./Assets/Border.png");
    renderer.LoadTexture("Background", "./Assets/Background.png");
    renderer.LoadTexture("YellowMiddleJade", "./Assets/YellowMiddleJade.png");
    renderer.LoadTexture("EnemySmallRandomer", "./Assets/EnemyShipGreenMiddle.png");
    renderer.LoadTexture("GreenBullet", "./Assets/GreenBullet.png");
    renderer.LoadTexture("EnemySmallSniper", "./Assets/EnemyShipGreyMiddle.png");
    renderer.LoadTexture("PurpleBullet45D", "./Assets/PurpleBullet45D.png");
    renderer.LoadTexture("EnemySmallFixer", "./Assets/EnemyShipRedMiddle.png");
    renderer.LoadTexture("RedBullet", "./Assets/RedBullet.png"); 
    renderer.LoadTexture("Score", "./Assets/Score.png");
    renderer.LoadTexture("Life", "./Assets/Life.png");
    renderer.LoadTexture("Power", "./Assets/Power.png");
    renderer.LoadTexture("Bomb", "./Assets/Bomb.png");
    renderer.LoadFont("UIFont", "./Assets/UIFont.ttf", 24);
    renderer.LoadTexture("LifeSmall", "./Assets/LifeSmall.png");
    renderer.LoadTexture("BombSmall", "./Assets/BombSmall.png");
    renderer.LoadTexture("SlowEffect", "./Assets/SlowEffect.png");
    renderer.LoadTexture("BulletEffect", "./Assets/BulletEffect.png");

    // 声明场景
    Scene scene;

    // 初始化 全局 Debug 组件
    auto debugObject = make_shared<GameObject>("DebugObject");
    debugObject->AddComponent(make_shared<Debug>(debugObject.get()));
    scene.AddObject(debugObject);

    // 注册预制体
    RegisterPlayerPrefab();
    RegisterBulletPrefab();
    RegisterEnemyPrefab();
    RegisterPlayerYellowBulletPrefab();
    RegisterBulletTemplatePrefab();
    RegisterEnemySmallRandomerPrefab();
    RegisterEnemyBezierSniper();
    RegisterEnemyBezierFixer();
    RegisterScoreItem();
    RegisterBombItem();
    RegisterLifeItem();
    RegisterPowerItem();
    RegisterScoreLabel();
    RegisterPowerLabel();
    RegisterLifeLabel();
    RegisterBombLabel();
    RegisterScrollBG();

    // 波数控制器 WaveController
    auto waveGameObject = std::make_shared<GameObject>("WaveControllerObject", Tag::None);
    auto waveController = std::make_shared<WaveController>(waveGameObject.get());
    waveGameObject->AddComponent(waveController);
    scene.AddObject(waveGameObject);

    std::string stagePath;
    if (argc >= 2)
    {
        stagePath = argv[1];
    }

    bool loaded = false;
    if (!stagePath.empty())
    {
        loaded = loadStageFromJson(stagePath, *waveController);
    }
    if (!loaded)
    {
        GenerateRandomStage(*waveController, 0);
    }

    /*
    // 构造波次
    Wave wave1;
    wave1.startTime = 0.0f;
    InitWave1(wave1);
    waveController->AddWave(wave1);

    Wave wave2;
    wave2.startTime = 4.0f;
    InitWave2(wave2);
    waveController->AddWave(wave2);

    Wave wave3;
    wave3.startTime = 10.0f;
    InitWave3(wave3);
    waveController->AddWave(wave3);

    Wave wave4;
    wave4.startTime = 24.0f;
    InitWave4(wave4);
    waveController->AddWave(wave4);
    */

    scene.InstantiatePrefab("player", Vector2(280.0f, 80.0f));
    scene.InstantiatePrefab("scoreLabel", Vector2(640.0f, 325.0f));
    scene.InstantiatePrefab("powerLabel", Vector2(640.0f, 355.0f));
    scene.InstantiatePrefab("lifeLabel", Vector2(625.0f, 470.0f));
    scene.InstantiatePrefab("bombLabel", Vector2(625.0f, 422.5f));
    scene.InstantiatePrefab("scrollBG", Vector2::Zero());
    scene.InstantiatePrefab("scrollBG", Vector2(0.0f, 800.0f));
    scene.InstantiatePrefab("scrollBG", Vector2(0.0f, 400.0f));
    
    // Border
    auto borderObject = make_shared<GameObject>("borderObject", Tag::UI);
    borderObject->transform.SetPosition(Vector2::Zero());
    auto borderObjectRenderer = make_shared<RendererComponent>(borderObject.get(), RenderKind::Sprite);
    borderObjectRenderer->SetSprite("Border");
    borderObjectRenderer->SetCenter(false);
    borderObjectRenderer->SetLayer(RenderLayer::UI);
    borderObject->AddComponent(borderObjectRenderer);
    scene.AddObject(borderObject);

    /*
    // 测试 Label
    auto labelObject = make_shared<GameObject>("label", Tag::UI);
    labelObject->transform.SetPosition(Vector2(280.f, 500.0f));
    auto labelObjectRenderer = make_shared<RendererComponent>(labelObject.get(), RenderKind::Text);
    labelObjectRenderer->SetFontKey("UIFont");
    labelObjectRenderer->SetText(std::string("Test"));
    labelObjectRenderer->SetLayer(RenderLayer::UI);
    labelObjectRenderer->SetOrderInLayer(10);
    labelObject->AddComponent(labelObjectRenderer);
    scene.AddObject(labelObject);
    */
    // BackGround


    // 测试 Item
    // auto itemTest = PrefabManager::Get().Instantiate("scoreItem");
    // itemTest->transform.SetPosition(280.0f, 550.0f);
    // scene.AddObject(itemTest);

    // Scene? 启动!
    scene.Start();

    bool running = true;
    while (running)
    {
        auto now = Clock::now();
        float deltaTime = std::chrono::duration<float>(now - last).count();
        last = now;

        if (deltaTime < 0.0f)
        {
            Debug::Log("[WARN] negative deltaTime corrected: " + std::to_string(deltaTime));
        }
        if (deltaTime > MAX_DT) 
        {   // 50 ms 阈值可调
            Debug::Log("[WARN] large dt=" + to_string(deltaTime) + " frame=" + to_string(tick));
            deltaTime = MAX_DT;
        }
        // if (tick % 60 == 0) Debug::Log("FPS: " + to_string(1/deltaTime));

        tick++;
        if (tick % 65536 == 0) tick = 0;

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) 
            {
                running = false;
            }
            else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                int w = e.window.data1;
                int h = e.window.data2;
                renderer.UpdateViewport(w, h);
            }
            scene.ProcessEvent(e);
        }
        
        scene.Update(deltaTime);
        scene.Render(renderer);

        auto frameEnd = Clock::now();
        float frameTime = std::chrono::duration<float>(frameEnd - now).count();
        float sleepTime = TARGET_FRAME - frameTime;
        if (sleepTime > 0.0f)
        {
            SDL_Delay((Uint32)(sleepTime * 1000.0f));
        }
    }

    scene.Destroy();
    BulletPool::Get().Clear();
    PrefabManager::Get().Clear();
    return 0;
}
