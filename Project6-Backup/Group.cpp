#include "Group.h"
#include <iostream>
#include <utility>
using namespace std;

/// @brief 
/// @param startingID 
/// @param countInput 
/// @param groupIDInput 
Group::Group(int startingID, const size_t& countInput, const size_t& groupIDInput)
{
    groupID = groupIDInput;
    count = countInput;
    students = StudentVector();
    decisions = new PlayType[count];
    playCount = 0;
    IDOfFirstStudent = startingID;

    for(int i = 0; i < countInput; i++)
    {
        students.PushBack(new Student(startingID));
        startingID++;
    }
}

Group::Group(int startingID, const size_t& countInput, const size_t& groupIDInput, StudentVector&& studentsInput)
{
    groupID = groupIDInput;
    count = countInput;
    // 直接接管传入的学生向量的所有权，避免拷贝
    students = std::move(studentsInput);
    decisions = new PlayType[count];
    playCount = 0;
    IDOfFirstStudent = startingID;
    // 确保 decisions 数组中未使用的条目不会导致未初始化访问
    for (size_t i = students.Length(); i < count; i++)
        decisions[i] = PlayType::NotPlay;
}

Group::~Group()
{
    delete[] decisions;
    students.DeleteStudents();
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

Group* Group::Split()
{
    if (count < 2) return nullptr;
    size_t firstHalfCount = count / 2;
    size_t lastHalfCount = count - firstHalfCount;

    //创建新组
    StudentVector lastHalfStudents = StudentVector();
    for (size_t i = 0; i < lastHalfCount; i++)
    {
        lastHalfStudents.PushBack(students[firstHalfCount + i]);
    }
    int newStartingID = IDOfFirstStudent + firstHalfCount;
    Group* newGroup = new Group(newStartingID, lastHalfCount, groupID + 1, std::move(lastHalfStudents));

    //创建前半部分数组
    StudentVector firstHalfStudents = StudentVector();
    for (size_t i = 0; i < firstHalfCount; i++)
    {
        firstHalfStudents.PushBack(students[i]);
    }

    students = std::move(firstHalfStudents);
    count = firstHalfCount;

    delete[] decisions;
    decisions = new PlayType[firstHalfCount];

    return newGroup;
}

Group& Group::operator+=(const Group& other)
{
    count += other.count;

    delete[] decisions;
    decisions = new PlayType[count];

    for (size_t i = 0; i < other.students.Length(); i++)
    {
        students.PushBack(new Student(*(other.students[i])));
    }

    playCount = 0;

    return *this;
}

ostream& operator<< (ostream& os, const Group& group)
{
    os << "Group " << group.groupID << ": " << group.count << " students, " << 
        group.playCount << " play, " << group.count - group.playCount << " not play" << endl;

    // Debug
    // os << "\t" << "Students: ";
    // for (int i = 0; i < group.students.Length(); i++)
    // {
    //     os << group.students[i]->GetID() << " ";
    // }
    // os << endl;
    // EndDebug

    os << "\t" << "ID to play: ";
    for (int i = 0; i < group.students.Length(); i++)
    {
        if (group.decisions[i] == PlayType::Play)
        {
            os << group.students[i]->GetID() << " ";
        }
    }


    return os;
}
