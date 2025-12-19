#ifndef _GAME_H_
#define _GAME_H_
#include <list>
#include <memory>
#include <string>
#include <vector>
#include "Vector2Int.h"
#include <unordered_map>
#include "Gui.h"
#include "Item.h"
#include "Grass.h"
#include "Animal.h"
using namespace std;

class Game {
private:
    Gui gui;
    unordered_map<Vector2Int, shared_ptr<Grass>, Vector2IntHash> grasses;
    unordered_map<Vector2Int, shared_ptr<Animal>, Vector2IntHash> animals;
    size_t count;

    size_t day = 0;

    string debugString = "";

    bool isGameEnd = false;
    
    bool IsPosValid (const Vector2Int& pos) const;
    bool IsPosTakenUpByAnimal (const Vector2Int& pos) const;
    bool IsPosInBorder (const Vector2Int& pos) const;

    template<typename Func>
    vector<Vector2Int> FliteredMoves(const vector<Vector2Int> originalMoves, Func filter);

    vector<Vector2Int> GetValidGrassPos() const;
    vector<Vector2Int> GetValidAnimalPos() const;

public:
    Game() ;
    ~Game() {gui.End();}
    void PlaceTokenAt(size_t r, size_t c, char x, int colorPair = 0, bool isBold = false);
    void ShowMessageAt(size_t r, size_t c, string s);
    void Update();
    void UpdateDebug();
    void UpdateUI();
    void UpdateItems(int c);
    void PlaceItem(const Item& item, bool isBold = false);
    Vector2Int GenerateRandomPos(vector<Vector2Int> validPos);
    bool IsGameEnd() { return isGameEnd; }
    void InitAnimals(int tigers, int wolves, int rabbits); // 新增
};

#endif
