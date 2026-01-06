#include "PowerLabel.h"
#include "../../Engine/Core/Scene/Scene.h"
#include "Player.h"
void PowerLabel::Start()
{
    playerRef = gameObject->GetScene()->FindFirstByNameInRoot("Player");
}

void PowerLabel::Update(float deltaTime)
{
    auto renderPtr = renderRef.lock();
    auto playerPtr = playerRef.lock();
    if (!playerPtr || !renderPtr)
    {
        renderPtr->SetText(lastPower);
        return;
    }
    auto playerScript = playerPtr->GetComponent<Player>();
    int power = playerScript->GetPower();
    int hundredDigit = power / 100;
    power -= hundredDigit * 100;
    lastPower = to_string(hundredDigit) + "." + ((power < 10)?"0":"") + to_string(power);
    renderPtr->SetText(lastPower);
}
