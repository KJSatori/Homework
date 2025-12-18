#include "Grass.h"

void Grass::Initialize()
{
    symbols[0] = ' ';
    symbols.push_back(' ');
    symbols.push_back('.');
    symbols.push_back(':');
    symbols.push_back('*');
    label = "Grass";
    type = AnimalType::Grass;
    colorPair = 1;
}

void Grass::Update(Vector2Int targetpos)
{
    if (state != GetMaxState()) state++;
}
