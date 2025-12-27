#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Script.h"
#include "../../GameObject/GameObject.h"
#include "../../../Math/Vector2.h"
#include "../Rigidbody.h"
#include "../InputHandler.h"
#include "../Debug.h"

struct PlayerController : public Script // 暂时改动
{
    shared_ptr<Rigidbody> rigidbody;
    shared_ptr<InputHandler> input;
    Vector2 dir = Vector2(0, 0);
    float maxVelocity;
    

    PlayerController(GameObject* owner, shared_ptr<Rigidbody> rb, shared_ptr<InputHandler> ih, float maxV)
        : Script(owner), rigidbody(rb), input(ih), maxVelocity(maxV) {}

    void RegisterInput()
    {
        dir = Vector2(0, 0);
        input->BindKey(
            KEY_W, 
            [this](bool down)
            { 
                if (down) dir.y = -1;
                // Debug::Log(Vector2(0, 5), std::string("W: ") + (down ? "Down" : "Release")); 
            }
        );
        input->BindKey(
            KEY_A, 
            [this](bool down)
            { 
                if (down) dir.x = -1;
                // Debug::Log(Vector2(0, 6), std::string("A: ") + (down ? "Down" : "Release")); 
            }
        );
        input->BindKey(
            KEY_S, 
            [this](bool down)
            { 
                if (down) dir.y = 1;
                // Debug::Log(Vector2(0, 3), std::string("S: ") + (down ? "Down" : "Release")); 
            }
        );
        input->BindKey(
            KEY_D, 
            [this](bool down)
            { 
                if (down) dir.x = 1;
                // Debug::Log(Vector2(0, 4), std::string("D: ") + (down ? "Down" : "Release")); 
            }
        );
    }
    void Update(float deltaTime) override 
    {
        // Debug::Log(Vector2(0, 1), "(" + to_string(dir.x) + ", " + to_string(dir.y)+ ")");
        if (dir != Vector2::Zero())
        {
            rigidbody->SetVelocity(dir.Normalized() * maxVelocity);
        }
        else
        {
            rigidbody->SetVelocity(Vector2::Zero());
        }
        dir = Vector2(0, 0);
    }

    void OnColliderEnter(GameObject* other) override
    {
        auto rb = other->GetComponent<Rigidbody>();
        if (rb)
        {
            // 简单反弹 
            Vector2 dir = other->transform.position - gameObject->transform.position;
            rb->velocity = dir.Normalized() * maxVelocity;
        }
    }
};


#endif