#include "LifeLabel.h"
#include "../../Engine/Core/Scene/Scene.h"
#include "Player.h"
void LifeLabel::Start()
{
    playerRef = gameObject->GetScene()->FindFirstByNameInRoot("Player");
}

void LifeLabel::Update(float deltaTime)
{
    auto player = playerRef.lock();
    if (!player) return;
    auto playerScirpt = player->GetComponent<Player>();
    int life = playerScirpt->GetHealth();
    int i = 0;
    for (; i < life; ++i)
    {
        auto render = renderRefs[i].lock();
        render->SetAlpha(255);
    }
    for (; i < 8; ++i)
    {
        auto render = renderRefs[i].lock();
        render->SetAlpha(0);
    }
}
