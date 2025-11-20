#ifndef WORLD_H
#define WORLD_H
#include "Group.h"
#include "Utils.h"

class World
{
private:
    Group** groups;
    size_t count;
    size_t capacity;
    Weather* weather;
    void Resize();
    void DebugInfo();
    bool TrySplitRandomGroup();
    bool TryMergeRandomGroup();
    int FindGroupToSplit();
    Position FindGroupToMerge();
    void PrintCurrState(int day);

public:
    World(size_t countInput);
    ~World();

    void DoSimulation();
    void AddGroup(Group* newGroup);

    Group* DebugGetGroup(int index);
};

#endif