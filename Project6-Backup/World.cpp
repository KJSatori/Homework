#include "World.h"
#include <iostream>
#include <vector>
using namespace std;

World::World(size_t countInput)
{
    count = countInput;
    groups = new Group*[count];
    capacity = count;

    int startingID = 0;
    int studentsCount = 30;
    int groupID = 0;
    for (int i = 0; i < count; i++)
    {
        groups[i] = new Group(startingID, studentsCount, groupID);
        startingID += studentsCount;
        groupID++;
    }

    weather = new Weather();
}

//寻找可分割的组（人数超过10）
int World::FindGroupToSplit()
{
    vector<int> spilitableGroups;
    for (int i = 0; i < count; i++)
    {
        if (groups[i]->GetStudentCount() > 10)
        {
            spilitableGroups.push_back(i);
        }
    }

    if (spilitableGroups.empty())
    {
        return -1;
    }

    int randomIndex = rand() % spilitableGroups.size();
    return spilitableGroups[randomIndex];
}

Position World::FindGroupToMerge()
{
    int xIndex = rand() % count;
    int yIndex = rand() % count;
    while (yIndex == xIndex) yIndex = rand() % count;
    Position output;
    output.x = xIndex;
    output.y = yIndex;
    return output;
}

World::~World()
{
    for(int i = 0; i < count; i++)
    {
        delete groups[i];
    }
    delete[] groups;
    delete weather;
}

void World::DoSimulation()
{
    for(int day = 1; day <= 30; day++)
    {
        weather->UpdateWeather();

        PrintCurrState(day);

        if (TrySplitRandomGroup())
        {
            DebugInfo();
        }
        if (TryMergeRandomGroup())
        {
            DebugInfo();
        }

        for(int i = 0; i < count; i++)
        {
            groups[i]->GroupDecision(weather->GetWeather());
            cout << *(groups[i]) << endl;
            //groups[i]->DebugReport();
        }
        cout << endl;
    }
}

Group* World::DebugGetGroup(int index)
{
    return groups[index];
}

void World::DebugInfo()
{
    cout << "当前组数: " << count <<" 容量: " << capacity << endl;
}

void World::Resize()
{
    int newCapacity = capacity * 2;
    Group** newGroups = new Group*[newCapacity];

    for (int i = 0; i < count; i++)
    {
        newGroups[i] = groups[i];
    }

    delete[] groups;
    groups = newGroups;
    capacity = newCapacity;
}

bool World::TrySplitRandomGroup()
{
    int groupIndex = FindGroupToSplit();
    if (GenerateRandf() < 0.2 && groupIndex >= 0)
    {
        Group* newGroup = groups[groupIndex]->Split();
        if (newGroup)
        {
            AddGroup(newGroup);

            for (int i = groupIndex + 2; i < count; i++)
            {
                groups[i]->SetGroupID(groups[i]->GetGroupID() + 1);
            }
            cout << "--- 组 " <<groups[groupIndex]->GetGroupID() <<
                " 被分割为组 " << groups[groupIndex]->GetGroupID() <<
                " 和组 " << newGroup->GetGroupID() << " ---" << endl;
            return true;
        }
    }
    return false;
}

bool World::TryMergeRandomGroup()
{
    Position groupsIndex = FindGroupToMerge();
    if (GenerateRandf() < 0.2)
    {
        int smallerIndex = (groupsIndex.x < groupsIndex.y) ? groupsIndex.x : groupsIndex.y;
        int largerIndex = (groupsIndex.x > groupsIndex.y) ? groupsIndex.x : groupsIndex.y;
        *(groups[smallerIndex]) += *(groups[largerIndex]);
        delete groups[largerIndex];

        // Debug
        // cout << "I will debug" << endl;
        // groups[smallerIndex]->GroupDecision(weather->GetWeather());
        // cout << *groups[smallerIndex] << endl;
        // groups[largerIndex] = groups[largerIndex+1];
        // groups[smallerIndex]->GroupDecision(weather->GetWeather());
        // cout << *groups[smallerIndex] << endl;
        // EndDebug


        for (int i = largerIndex; i < count - 1; i++)
        {
            groups[i] = groups[i+1];
            groups[i]->SetGroupID(groups[i]->GetGroupID() - 1);
        }
        count--;
        cout << "--- 组 " << groups[smallerIndex]->GetGroupID() << " 和组 " << 
            groups[largerIndex]->GetGroupID() - 1 << " 被合并为组 " << groups[smallerIndex]->GetGroupID()  << " ---" << endl;
        return true;
    }
    return false;
}

void World::AddGroup(Group* newGroup)
{
    if (count >= capacity)
    {
        Resize();
    }

    int insertIndex = -1;
    for (int i = 0; i < count; i++)
    {
        if (groups[i]->GetGroupID() == newGroup->GetGroupID() - 1)
        {
            insertIndex = i + 1;
            break;
        }
    }

    if (insertIndex == -1)
    {
        groups[count] = newGroup;
    }
    else
    {
        for (int i = count; i > insertIndex; i--)
        {
            groups[i] = groups[i - 1];
        }
        groups[insertIndex] = newGroup;
    }
    count++;
}

void World::PrintCurrState(int day)
{
    cout << "-------------- Day: " << day << " | Weather: ";
    switch (weather->GetWeather())
    {
        case WeatherType::SunnyAndHumid:
            cout << "Sunny and Humid";
            break;
        case WeatherType::SunnyAndNotHumid:
            cout << "Sunny and not Humid";
            break;
        case WeatherType::RainingAndWindy:
            cout << "Raining and Windy";
            break;
        case WeatherType::RainingAndNotWindy:
            cout << "Raining and not Windy";
            break;
        case WeatherType::Overcast:
            cout << "Overcast";
            break;
    }
    cout << " ---------------" << endl;
}