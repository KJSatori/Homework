#ifndef GRASS_H
#define GRASS_H

#include "Animal.h"

class Grass: public Animal
{
private:
    void Initialize() override;
public:
    Grass(int r, int c): Animal(r, c) { Initialize(); }
    ~Grass() {};

    void Update(Vector2Int targetPos = Vector2Int(0, 0)) override;
    void Eat() override {}
    vector<Vector2Int> PossibleMoves() override { return vector<Vector2Int>(); }
    vector<Vector2Int> PossibleMoves(unordered_map<Vector2Int, shared_ptr<Animal>, Vector2IntHash>) override { return vector<Vector2Int>(); }
    void MoveTo(Vector2Int targetPos = Vector2Int(0, 0)) override {}

    
};


#endif