#ifndef SCORELABEL_H
#define SCORELABEL_H

#include "../../Engine/Core/Component/Script.h"
#include "../../Engine/Core/Component/RendererComponent.h"
#include "../../Engine/Render/Renderer.h"
#include "../../Engine/Math/Vector2.h"
#include <memory>

class ScoreLabel : public Script 
{
private:
    std::weak_ptr<GameObject> playerRef;
    std::weak_ptr<RendererComponent> renderRef;
    int lastScore = 0;
public:
    ScoreLabel(GameObject* owner, std::shared_ptr<RendererComponent> rc) : Script(owner), renderRef(rc) {}
    void Start() override;
    void Update(float deltaTime) override;
};

#endif