#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Weather.h"
#include "Utils.h"
#include "Group.h"
#include "World.h"
#include "StudentVector.h"
using namespace std;

// -------------------------------------------------------
// 我没有直接复制唐教授的代码
// 因为我认为我原先的代码我自己更好修改且易读
// 部分代码我用了一些课上没讲的知识
// 不过我都是知道才会用的，因为我觉得这么写更科学
// 比如StudentVector的初始化函数我会使用initializer_list
// -------------------------------------------------------

int main()
{  
    srand((unsigned int)time(NULL));

    World world = World(20);
    world.DoSimulation();

    return 0;

}

// 现在是0:50 经过一整天的艰苦奋斗
// 终于完美地，没有内存泄露，测试几十次都没有