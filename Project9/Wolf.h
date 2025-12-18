#ifndef WOLF_H
#define WOLF_H

#include "Animal.h"

class Wolf: public Animal
{
private:
    void Initialize() override;

public:
    Wolf(int r, int c): Animal(r, c) { Initialize(); }

    void Update(Vector2Int targetPos = Vector2Int(0, 0)) override;
    void Eat() override;
    vector<Vector2Int> PossibleMoves() override;
    vector<Vector2Int> PossibleMoves(unordered_map<Vector2Int, shared_ptr<Animal>, Vector2IntHash>) override;
    void MoveTo(Vector2Int targetPos) override;
};

#endif