#ifndef BOMBLABEL_H
#define BOMBLABEL_H

#include "../../Engine/Core/Component/Script.h"
#include "../../Engine/Core/Component/RendererComponent.h"
#include "../../Engine/Render/Renderer.h"
#include "../../Engine/Math/Vector2.h"
#include <memory>
#include <vector>

class BombLabel : public Script
{
private:
    std::weak_ptr<GameObject> playerRef;
    std::vector<std::weak_ptr<RendererComponent>> renderRefs;
public:
    BombLabel(GameObject* owner, std::vector<std::shared_ptr<RendererComponent>> rcs) : Script(owner) 
    {
        for (const auto& sp : rcs)
        {
            renderRefs.push_back(sp);
        }
    }
    void Start() override;
    void Update(float deltaTime) override;
};

#endif