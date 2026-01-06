#ifndef BACKGROUNDSCROLLER_H
#define BACKGROUNDSCROLLER_H

#include "../../Engine/Core/GameObject/GameObject.h"
#include "../../Engine/Core/Component/RendererComponent.h"
#include "../../Engine/Core/Component/Script.h"

class BackGroundScroller : public Script
{
    const float velocity = 100.0f;
public:
    BackGroundScroller(GameObject* owner) : Script(owner) {}
    void Update(float deltaTime) override
    {
        gameObject->transform.Translate(Vector2(0.0f, -velocity * deltaTime));
        if (gameObject->transform.position.y <= -600.0f)
        {
            gameObject->transform.SetPosition(Vector2(0.0f, 600.0f));
        }
    }
};

#endif