#include "BombLabel.h"
#include "../../Engine/Core/Scene/Scene.h"
#include "Player.h"
void BombLabel::Start()
{
    playerRef = gameObject->GetScene()->FindFirstByNameInRoot("Player");
}

void BombLabel::Update(float deltaTime)
{
    auto player = playerRef.lock();
    if (!player) return;
    auto playerScirpt = player->GetComponent<Player>();
    int bomb = playerScirpt->GetBomb();
    int i = 0;
    for (; i < bomb; ++i)
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
