#include "Group.h"
#include <iostream>
using namespace std;

/// @brief 
/// @param startingID 
/// @param countInput 
/// @param groupIDInput 
Group::Group(int startingID, const size_t& countInput, const size_t& groupIDInput)
{
    groupID = groupIDInput;
    count = countInput;
    students = new Student*[count];
    decisions = new PlayType[count];
    playCount = 0;
    IDOfFirstStudent = startingID;

    for(int i = 0; i < countInput; i++)
    {
        students[i] = new Student(startingID);
        startingID++;
    }
}

Group::Group(int startingID, const size_t& countInput, const size_t& groupIDInput, Student** studentsInput)
{
    groupID = groupIDInput;
    count = countInput;
    students = new Student*[count];
    decisions = new PlayType[count];
    playCount = 0;
    IDOfFirstStudent = startingID;
    for(int i = 0; i < countInput; i++)
    {
        students[i] = studentsInput[i];
    }
}

Group::~Group()
{
    for(int i = 0; i < count; i++)
    {
        delete students[i];
    }
    delete[] students;
}

void Group::GroupDecision(WeatherType weather)
{
    playCount = 0;
    for(int i = 0; i < count; i++)
    {
        decisions[i] = students[i] -> MakeDecision(weather);
        if (decisions[i] == PlayType::Play) 
        {
            playCount++;
        }
    }
}

void Group::Report() const
{
    cout << "Group " << groupID << ": " << count << " students, " << 
        playCount << " play, " << count - playCount << " not play" << endl;
    cout << "\t" << "ID to play: ";
    for (int i = 0; i < count; i++)
    {
        if (decisions[i] == PlayType::Play)
        {
            cout << IDOfFirstStudent + i << " ";
        }
    }
    cout << endl;
}

void Group::DebugReport() const
{
    for (int i = 0; i < count; i++)
    {
        students[i]->DebugGetInfo();
    }
}

Group* Group::Split()
{
    if (count < 2) return nullptr;
    size_t firstHalfCount = count / 2;
    size_t lastHalfCount = count - firstHalfCount;

    //创建新组
    Student** lastHalfStudents = new Student*[lastHalfCount];
    for (size_t i = 0; i < lastHalfCount; i++)
    {
        lastHalfStudents[i] = students[firstHalfCount + i];
    }
    int newStartingID = IDOfFirstStudent + firstHalfCount;
    Group* newGroup = new Group(newStartingID, lastHalfCount, groupID + 1, lastHalfStudents);

    //创建前半部分数组
    Student** firstHalfStudents = new Student*[firstHalfCount];
    for (size_t i = 0; i < firstHalfCount; i++)
    {
        firstHalfStudents[i] = students[i];
    }

    //删除原来数组
    delete[] students;
    students = firstHalfStudents;
    count = firstHalfCount;

    delete[] decisions;
    decisions = new PlayType[firstHalfCount];

    return newGroup;
}