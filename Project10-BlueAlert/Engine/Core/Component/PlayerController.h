#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Script.h"
#include "../GameObject/GameObject.h"
#include "../../Math/Vector2.h"
#include "Rigidbody.h"
#include "InputHandler.h"
#include "Debug.h"
#include "../../../Game/Scripts/Player.h"

struct PlayerController : public Script
{
    shared_ptr<Rigidbody> rigidbody;
    shared_ptr<InputHandler> input;
    shared_ptr<Player> player;
    Vector2 dir = Vector2(0, 0);
    float maxVelocity;

    // 用于调试跳变bug
    Vector2 debugPrev;
    Vector2 debugCurr;
    
    PlayerController(GameObject* owner, shared_ptr<Rigidbody> rb, shared_ptr<InputHandler> ih, float maxV)
        : Script(owner), rigidbody(rb), input(ih), maxVelocity(maxV) {}
        
    void Start() override
    {
        player = gameObject->GetComponent<Player>();
        if (!player) Debug::Log("[PlayerController] 未找到player脚本");
        RegisterInput();
    }

    void RegisterInput()
    {
        auto tempPlayer = player;
        dir = Vector2(0, 0);
        input->BindKey(
            SDLK_UP, 
            [this]() { dir.y += 1; },
            [this]() { dir.y -= 1; }
        );
        input->BindKey(
            SDLK_LEFT, 
            [this]() { dir.x -= 1; },
            [this]() { dir.x += 1; }
        );
        input->BindKey(
            SDLK_DOWN, 
            [this]() { dir.y -= 1; },
            [this]() { dir.y += 1; }
        );
        input->BindKey(
            SDLK_RIGHT, 
            [this]() { dir.x += 1; },
            [this]() { dir.x -= 1; }
        );
        if (!tempPlayer) return;
        input->BindKey(
            SDLK_z,
            [tempPlayer]() { tempPlayer->ContinuouslyFire(true); },
            [tempPlayer]() { tempPlayer->ContinuouslyFire(false); }
        );
        input->BindKey(
            SDLK_LSHIFT,
            [tempPlayer, this]() { maxVelocity = tempPlayer->GetVelocity(true); tempPlayer->SetSlowEffectVislble(true); },
            [tempPlayer, this]() { maxVelocity = tempPlayer->GetVelocity(false); tempPlayer->SetSlowEffectVislble(false); }
        );
        input->BindKey(
            SDLK_x,
            [tempPlayer, this]() { tempPlayer->UseBomb(); },
            [tempPlayer, this]() { return; }
        );
    }

    void Update(float deltaTime) override 
    {
        debugPrev = gameObject->transform.position;

        Vector2 velocity = Vector2::Zero();
        // Debug::Log("(" + to_string(rigidbody->GetVelocity().x) + ", " + to_string(rigidbody->GetVelocity().y) + ")");
        
        if (dir != Vector2::Zero() && !player->IsDuringSpawn())
        {
            velocity = dir.Normalized() * maxVelocity;
        }

        Vector2 nextPos = gameObject->transform.position + velocity * deltaTime;
        if (nextPos.x < 50.0f || nextPos.x > 510.0f) velocity.x = 0;
        if (nextPos.y < 40.0f || nextPos.y > 565.0f) velocity.y = 0;

        rigidbody->SetVelocity(velocity);

        debugCurr = gameObject->transform.position;
        float dist = (debugCurr - debugPrev).Magnitude();
        if (dist > maxVelocity * deltaTime * 1.1f)
        {
            Debug::Log(
                "[Teleport] obj=" + gameObject->name +
                " addr=" + to_string((uintptr_t)gameObject) +
                " prev=" + string(debugPrev) + " cur=" + string(debugCurr) +
                " dist=" + to_string(dist) + " dt = " + to_string(deltaTime) +
                " vel=" + to_string(maxVelocity)
            );
        }
    }

    void OnColliderEnter(GameObject* other) override
    {
        /* 测试脚本
        auto rb = other->GetComponent<Rigidbody>();
        if (rb)
        {
            // 简单反弹 
            Vector2 dir = other->transform.position - gameObject->transform.position;
            rb->velocity = dir.Normalized() * maxVelocity;
        }
        */
    }
};


#endif