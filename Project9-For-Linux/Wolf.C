#include "Wolf.h"

void Wolf::Initialize()
{
    fixedLife = 10;
    lifecycle = fixedLife;
    symbols[0] = 'W';
    label = "Wolf";
    type = AnimalType::Wolf;
    colorPair = 3;
}

void Wolf::Update(Vector2Int targetPos)
{
    if (targetPos != Vector2Int(0, 0)) MoveTo(targetPos);
    // 减少生命周期
    lifecycle--;
}

void Wolf::Eat()
{
    lifecycle = fixedLife;
}

vector<Vector2Int> Wolf::PossibleMoves()
{
    vector<Vector2Int> possiblePoses = vector<Vector2Int>();
    for (int i = 1; i <= 24; i++)
    {
        possiblePoses.push_back(pos + GenerateDirectionPosByDirection(i));
    }
    return possiblePoses;
}

vector<Vector2Int> Wolf::PossibleMoves(unordered_map<Vector2Int, shared_ptr<Animal>, Vector2IntHash> animals)
{
    vector<Vector2Int> possiblePoses = vector<Vector2Int>();
    bool hasFood = false;

    for (auto& [key, value]: animals)
    {
        auto animal = value;
        if (IsPosInSelfborder(animal->GetPos(), 2))
        {
            if (animal->GetType() == AnimalType::Rabbit)
            {
                hasFood = true;
                possiblePoses.push_back(animal->GetPos());
            }
        }
    }

    if (!hasFood)
    {
        for (int i = 1; i <=24; i++)
        {
            possiblePoses.push_back(pos + GenerateDirectionPosByDirection(i));
        }
    }
    return possiblePoses;
}

void Wolf::MoveTo(Vector2Int targetPos)
{
    pos = targetPos;
}
