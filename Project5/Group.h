#ifndef GROUP_H
#define GROUP_H
#include <iostream>
#include "Student.h"
#include "Weather.h"

class Group
{
private:
    //一个含有N个学生指针的数组
    Student** students;
    size_t count;
    size_t playCount;
    PlayType* decisions;

    size_t groupID;
    int IDOfFirstStudent;
public:
    Group(int startingID, const size_t& count, const size_t& groupID);
    Group(int startingID, const size_t& count, const size_t& groupID, Student** students);
    ~Group();
    //Group(const Group& other);
    //Group& operator=(const Group& other);

    void GroupDecision(WeatherType weather);
    void Report() const;

    void DebugReport() const;

    Group* Split();
    size_t GetStudentCount() const { return count; }
    size_t GetGroupID() const { return groupID; }
    void SetGroupID(size_t newID) { groupID = newID; } 
};

#endif