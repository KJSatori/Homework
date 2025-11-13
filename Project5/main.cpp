#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Weather.h"
#include "Utils.h"
#include "Group.h"
#include "World.h"
using namespace std;

int main()
{  
    srand((unsigned int)time(NULL));

    World world = World(20);
    world.DoSimulation();

    return 0;
}