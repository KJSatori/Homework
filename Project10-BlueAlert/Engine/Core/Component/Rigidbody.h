#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Component.h"
#include "../GameObject/GameObject.h"
#include "../../Render/Renderer.h"
#include "../../Math/Vector2.h"
#include "Debug.h"

struct Rigidbody : public Component
{
    Vector2 velocity;
    Vector2 acceleration;
    float mass;

    Rigidbody(GameObject* owner) : 
        Component(owner), mass(1.0f), velocity(Vector2::Zero()), acceleration(Vector2::Zero()) {}
    Rigidbody(GameObject* owner, float m) :
        Component(owner), mass(m), velocity(Vector2::Zero()), acceleration(Vector2::Zero()) {} 
    Rigidbody(GameObject* owner, const Vector2& v,
        const Vector2& a = Vector2::Zero(), float m = 1.0f) :
        Component(owner), velocity(v), acceleration(a), mass(m) {} 

    void AddForce(Vector2 force) { acceleration = force / mass; }
    void SetVelocity(Vector2 v) { velocity = v; }
    Vector2 GetVelocity() const { return velocity; }

    void Update(float deltaTime) override
    {
        velocity += deltaTime * acceleration;
        // if (velocity != Vector2::Zero()) Debug::Log(Vector2(0, 0), std::string("velocity: ") + to_string(deltaTime*velocity.x) + ", " + to_string(deltaTime*velocity.y));
        gameObject->transform.Translate(deltaTime * velocity);

        // 每帧清空加速度
        acceleration = Vector2(0, 0);
    }
};


#endif

