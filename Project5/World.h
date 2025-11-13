#ifndef WORLD_H
#define WORLD_H
#include "Group.h"

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
    int FindGroupToSplit();

public:
    World(size_t countInput);
    ~World();

    void DoSimulation();
    void AddGroup(Group* newGroup);

    Group* DebugGetGroup(int index);
};

#endif