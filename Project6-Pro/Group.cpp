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
    for (size_t i = 0; i < count; ++i) decisions[i] = PlayType::NotPlay;
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
    // 直接传入的学生向量的所有权，避免拷贝
    students = std::move(studentsInput);
    decisions = new PlayType[count];
    for (size_t i = 0; i < count; ++i) decisions[i] = PlayType::NotPlay;
    playCount = 0;
    IDOfFirstStudent = startingID;
    // 确保 decisions 数组中未使用的地方不会导致被错误访问
}

Group::~Group()
{
    delete[] decisions;
    students.DeleteStudents();
}

void Group::GroupDecision(WeatherType weather)
{
    playCount = 0;
    size_t available = students.Length();
    size_t n = (available < count) ? available : count;
    for (size_t i = 0; i < n; i++)
    {
        if (students[i] == nullptr) { decisions[i] = PlayType::NotPlay; continue; }
        decisions[i] = students[i]->MakeDecision(weather);
        if (decisions[i] == PlayType::Play) ++playCount;
    }
    // 如果 students 数量少于 count，调整 count 以反映真实人数
    if (available < count) count = available;
}

void Group::Report() const
{
    size_t available = students.Length();
    size_t n = (available < count) ? available : count;
    cout << "Group " << groupID << ": " << n << " students, " << 
        playCount << " play, " << n - playCount << " not play" << endl;
    cout << "\t" << "ID to play: ";
    for (size_t i = 0; i < n; i++)
    {
        if (decisions[i] == PlayType::Play)
        {
            cout << IDOfFirstStudent + (int)i << " ";
        }
    }
    cout << endl;
}

Group* Group::Split()
{
    if (count < 2) return nullptr;
    size_t firstHalfCount = count / 2;
    size_t lastHalfCount = count - firstHalfCount;

    // 从当前向量中提取后半部分，转移所有权到新组
    StudentVector lastHalfStudents = students.ExtractRange(firstHalfCount, lastHalfCount);
    int newStartingID = IDOfFirstStudent + firstHalfCount;
    Group* newGroup = new Group(newStartingID, lastHalfCount, groupID + 1, std::move(lastHalfStudents));

    // 当前 students 已由 ExtractRange 调整为前半部分
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
    for (size_t i = 0; i < group.students.Length(); i++)
    {
        if (i >= group.count) break;
        if (group.decisions[i] == PlayType::Play && group.students[i] != nullptr)
        {
            os << group.students[i]->GetID() << " ";
        }
    }


    return os;
}
