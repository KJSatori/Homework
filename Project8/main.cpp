#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "PQ.h"
using namespace std;

int main()
{
    srand(time(0));
    PQ<int> intPQ = PQ<int>();
    for (int i = 0; i < 16; i++)
    {
        int num = rand();
        // cout << i << ": " << num << endl;
        intPQ.Push(num);
    }
    while (!intPQ.Empty())
    {
        cout << intPQ.Pop() << " ";
    }
    cout << endl;

    PQ<double> doublePQ = PQ<double>();
    for (int i = 0; i < 16; i++)
    {
        doublePQ.Push((double)(rand()%1000)/((double)1000));
    }

    while (!doublePQ.Empty())
    {
        cout << doublePQ.Pop() << " ";
    }
    cout << endl;
    return 0;
}
