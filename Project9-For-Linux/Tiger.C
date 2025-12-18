#include "Tiger.h"

void Tiger::Initialize()
{
    fixedLife = 18;
    lifecycle = fixedLife;
    symbols[0] = 'T';
    label = "Tiger";
    type = AnimalType::Tiger;
    colorPair = 4;
}

void Tiger::Update(Vector2Int targetPos)
{
    if (targetPos != Vector2Int(0, 0)) MoveTo(targetPos);
    // 减少生命周期
    lifecycle--;
}

void Tiger::Eat()
{
    lifecycle = fixedLife;
}

vector<Vector2Int> Tiger::PossibleMoves()
{
    vector<Vector2Int> possiblePoses = vector<Vector2Int>();
    for (int i = 1; i <= 48; i++)
    {
        possiblePoses.push_back(pos + GenerateDirectionPosByDirection(i));
    }
    return possiblePoses;
}

vector<Vector2Int> Tiger::PossibleMoves(unordered_map<Vector2Int, shared_ptr<Animal>, Vector2IntHash> animals)
{
    vector<Vector2Int> possiblePoses = vector<Vector2Int>();
    bool hasFood = false;

    for (auto& [key, value]: animals)
    {
        auto animal = value;
        if (IsPosInSelfborder(animal->GetPos(), 3))
        {
            if (animal->GetType() == AnimalType::Rabbit || animal->GetType() == AnimalType::Wolf)
            {
                hasFood = true;
                possiblePoses.push_back(animal->GetPos());
            }
        }
    }

    if (!hasFood) for (int i = 1; i <= 48; i++) possiblePoses.push_back(pos + GenerateDirectionPosByDirection(i));
    return possiblePoses;
}

void Tiger::MoveTo(Vector2Int targetPos)
{
    pos = targetPos;
}
