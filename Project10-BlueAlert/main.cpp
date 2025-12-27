#define _XOPEN_SOURCE_EXTENDED 1
#include <locale.h>
#include <ncursesw/ncurses.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "Engine/Math/Vector2.h"
#include "Engine/Math/Transform.h"
#include "Engine/Render/NcursesRenderer.h"
#include "Engine/Core/GameObject/GameObject.h"
#include "Engine/Core/Component/RendererComponent.h"
#include "Engine/Core/Component/Rigidbody.h"
#include "Engine/Core/Component/Collider.h"
#include "Engine/Core/Component/InputHandler.h"
#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/Component/Scripts/PlayerController.h"

int main() {
    auto last = chrono::high_resolution_clock::now();

    NcursesRenderer renderer; 

    long long tick = 0;

    Scene scene;
    auto debugObject = make_shared<GameObject>();
    debugObject->AddComponent(make_shared<Debug>(debugObject.get()));
    scene.AddObject(debugObject);

    auto player = make_shared<GameObject>();
    player->transform.position = Vector2(12, 10);
    auto col = make_shared<Collider>(player.get());
    auto input = make_shared<InputHandler>(player.get());
    auto rb = make_shared<Rigidbody>(player.get());
    auto playerController = make_shared<PlayerController>(player.get(), rb, input, 20.0f);
    col->SetCircle(player->transform.position, 0.5f);
    player->AddComponent(col);
    player->AddComponent(make_shared<RendererComponent>(player.get(), L"我"));
    player->AddComponent(rb);
    player->AddComponent(input);
    player->AddComponent(playerController);
    playerController->RegisterInput();

    auto solar = make_shared<GameObject>();
    solar->AddComponent(make_shared<RendererComponent>(solar.get(), L"日"));

    auto earth = make_shared<GameObject>();
    auto earthSprite = make_shared<GameObject>();
    earthSprite->AddComponent(make_shared<RendererComponent>(earthSprite.get(), L"地"));

    auto moon = make_shared<GameObject>();
    moon->AddComponent(make_shared<RendererComponent>(moon.get(), L"月"));

    auto mars = make_shared<GameObject>();
    mars->AddComponent(make_shared<RendererComponent>(mars.get(), L"月"));

    solar->transform.SetPosition(Vector2(30, 15));
    earth->transform.SetPosition(solar->transform.position);
    earthSprite->transform.SetPosition(earth->transform.position + Vector2(5, 0));

    for (int i = 0; i < 10; i++)
    {
        auto ball = make_shared<GameObject>();
        ball->transform.SetPosition(Vector2(20 + i * 3, 12));
        auto ballCol = make_shared<Collider>(ball.get());
        ballCol->SetCircle(ball->transform.position, 0.5f);
        ball->AddComponent(ballCol);

        auto ballRb = make_shared<Rigidbody>(ball.get());
        ball->AddComponent(ballRb);
        ball->AddComponent(make_shared<RendererComponent>(ball.get(), L"球"));

        scene.AddObject(ball);
    }

    scene.AddObject(player);
    scene.AddObject(solar);
    scene.AddObject(earth);
    scene.AddObject(earthSprite);

    scene.Start();
    for (;;)
    {
        auto now = chrono::high_resolution_clock::now();
        float deltaTime = chrono::duration<float>(now - last).count();
        last = now;

        tick++;
        if (tick % 65536 == 0) tick = 0;
        // if (tick % 60 == 0) Debug::Log("FPS: " + to_string(1 / deltaTime));

        earth->transform.Rotate(90.0f * deltaTime);
        earthSprite->transform.SetPosition(earth->transform.TransformPoint(Vector2(5, 0)));
        
        scene.Update(deltaTime);
        scene.Render(renderer);

        this_thread::sleep_for(chrono::milliseconds(1));
    }

    scene.Destroy();

    getch();
    return 0;
}
