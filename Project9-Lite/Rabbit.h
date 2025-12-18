#ifndef RABBIT_H
#define RABBIT_H

#include "Animal.h"

class Rabbit: public Animal
{
private:
    void Initialize() override;

public:
    Rabbit(int r, int c): Animal(r, c) { Initialize(); }

    void Update(Vector2Int targetPos = Vector2Int(0, 0)) override;
    void Eat() override;
    vector<Vector2Int> PossibleMoves() override;
    vector<Vector2Int> PossibleMoves(unordered_map<Vector2Int, shared_ptr<Animal>, Vector2IntHash>) override;
    void MoveTo(Vector2Int targetPos) override;
};

#endif