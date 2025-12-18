#ifndef _ITEM_H_
#define _ITEM_H_

#define MINROWPOS 5
#define MAXROWPOS 23
#define MINCOLPOS 1
#define MAXCOLPOS 78 

#include <iostream>
#include <vector>
#include "Vector2Int.h"

using namespace std;

class Item {
    // 不允许实例化一个item 所以把 构造析构都定义为protected
protected:
    Vector2Int pos;
    vector<char> symbols = vector<char>(1, '?');
    string label = "???";
    int state = 0;

    // 生命周期，一旦这个数字变为0，将会被销毁，-1代表永远不会被销毁
    int lifecycle = -1;
    int fixedLife = -1;
    // 颜色
    int colorPair = 0;

    virtual void Initialize() = 0; // 强制子类实现

    Item(): pos(Vector2Int(0, 0)) {  }
    Item(Vector2Int newPos): pos(newPos) {  }
    Item(int r, int c): pos(Vector2Int(r, c)) {  }

    Vector2Int GenerateRandomPos()
    {
        return Vector2Int(rand()%20+4, rand()%78+1);
    }

    Vector2Int GenerateDirectionPosByDirection(int dir);
    bool IsPosInSelfborder(Vector2Int pos, int size);
    bool IsPosInBorder(Vector2Int pos);

public:
    virtual void Update() {};

    int GetLifecycle() const { return lifecycle; }
    char GetSymbol() const { return symbols[state]; }
    char GetMaxState() const { return symbols.size() - 1; }
    string GetLabel() const { return label; }
    Vector2Int GetPos() const { return pos; }
    int GetState() const { return state; }
    int GetColorPair() const { return colorPair; }

    void SetPos(int row, int col) { pos = Vector2Int(row, col); }
    void SetPos(Vector2Int newPos) { pos = newPos; }
    void SetLifeCycle(int lc) { lifecycle = lc; }
    void SetColorPair(int cp) { colorPair = cp; }
    void Destroy() { lifecycle = 0; }

    void ResetState() { state = 0; }

    virtual ~Item() {}
};
#endif
