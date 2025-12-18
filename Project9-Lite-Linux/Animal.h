#ifndef ANIMAL_H
#define ANIMAL_H

#include "Item.h"
#include "AnimalType.h"
#include <memory>

class Animal: public Item
{
protected:
    AnimalType type = AnimalType::Empty;

    Animal(): Item() {}
    Animal(int r, int c): Item(r, c) {}
    Animal(int r, int c, int s): Item(r, c) { state = s; } 

public:
    AnimalType GetType() { return type; }

    virtual void Eat() {}
    virtual void Update(Vector2Int targetPos = Vector2Int(0, 0)) {}
    virtual vector<Vector2Int> PossibleMoves() { return vector<Vector2Int>(); }
    virtual vector<Vector2Int> PossibleMoves(unordered_map<Vector2Int, shared_ptr<Animal>, Vector2IntHash>) { return vector<Vector2Int>(); }
    virtual void MoveTo(Vector2Int targetPos) {};
};

#endif