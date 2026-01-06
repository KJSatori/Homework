#ifndef POWERLABEL_H
#define POWERLABEL_H

#include "../../Engine/Core/Component/Script.h"
#include "../../Engine/Core/Component/RendererComponent.h"
#include "../../Engine/Render/Renderer.h"
#include "../../Engine/Math/Vector2.h"
#include <memory>

class PowerLabel : public Script 
{
private:
    std::weak_ptr<GameObject> playerRef;
    std::weak_ptr<RendererComponent> renderRef;
    string lastPower = "";
public:
    PowerLabel(GameObject* owner, std::shared_ptr<RendererComponent> rc) : Script(owner), renderRef(rc) {}
    void Start() override;
    void Update(float deltaTime) override;
};

#endif