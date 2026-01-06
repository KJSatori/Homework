#include "ScoreLabel.h"
#include "../../Engine/Core/Scene/Scene.h"
#include "Player.h"
void ScoreLabel::Start()
{
    playerRef = gameObject->GetScene()->FindFirstByNameInRoot("Player");
}

void ScoreLabel::Update(float deltaTime)
{
    auto renderPtr = renderRef.lock();
    auto playerPtr = playerRef.lock();
    if (!playerPtr || !renderPtr)
    {
        renderPtr->SetText(to_string(lastScore));
        return;
    }
    auto playerScript = playerPtr->GetComponent<Player>();
    lastScore = playerScript->GetScore();
    renderPtr->SetText(to_string(lastScore));
}
