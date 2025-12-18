#include "Rabbit.h"

void Rabbit::Initialize()
{
    fixedLife= 3;
    lifecycle = fixedLife;
    symbols[0] = 'R';
    label = "Rabbit";
    type = AnimalType::Rabbit;
    colorPair = 2;
}

void Rabbit::Update(Vector2Int targetPos)
{
    if (targetPos != Vector2Int(0, 0)) MoveTo(targetPos);
    // 减少生命周期
    lifecycle--;
}

void Rabbit::Eat()
{
    lifecycle = fixedLife;
}

vector<Vector2Int> Rabbit::PossibleMoves()
{
    vector<Vector2Int> possiblePoses = vector<Vector2Int>();
    for (int i = 1; i <=8; i++)
    {
        possiblePoses.push_back(pos + GenerateDirectionPosByDirection(i));
    }
    return possiblePoses;
}

vector<Vector2Int> Rabbit::PossibleMoves(unordered_map<Vector2Int, shared_ptr<Animal>, Vector2IntHash> discard)
{
    vector<Vector2Int> possiblePoses = vector<Vector2Int>();
    for (int i = 1; i <=8; i++)
    {
        possiblePoses.push_back(pos + GenerateDirectionPosByDirection(i));
    }
    return possiblePoses;
}

void Rabbit::MoveTo(Vector2Int targetPos)
{
    pos = targetPos;
}
