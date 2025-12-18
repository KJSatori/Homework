#ifndef _NOODLE_H_
#define _NOODLE_H_
#include "Item.h"
#include <iostream>
using namespace std;
class Noodle: public Item {
private:
    void Initialize() override;

public:
    Noodle(int r, int c): Item(r, c) { Initialize(); }
};
#endif
