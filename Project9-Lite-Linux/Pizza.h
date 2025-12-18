#ifndef _PIZZA_H_
#define _PIZZA_H_
#include "Item.h"
#include <iostream>
using namespace std;
class Pizza: public Item {
private:
    void Initialize() override;
public:
    Pizza(int r, int c): Item(r, c) { Initialize(); }
};
#endif
