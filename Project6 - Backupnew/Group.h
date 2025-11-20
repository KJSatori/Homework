#ifndef GROUP_H
#define GROUP_H
#include <iostream>
#include "Student.h"
#include "Weather.h"
#include "StudentVector.h"
using namespace std;

class Group
{
    friend ostream& operator<< (ostream& os, const Group& group);
private:
    StudentVector students;
    size_t count;
    size_t playCount;
    PlayType* decisions;

    size_t groupID;
    int IDOfFirstStudent;
public:
    Group(int startingID, const size_t& count, const size_t& groupID);
    Group(int startingID, const size_t& count, const size_t& groupID, StudentVector&& students);
    ~Group();

    void GroupDecision(WeatherType weather);
    void Report() const;

    Group* Split();
    size_t GetStudentCount() const { return count; }
    size_t GetGroupID() const { return groupID; }
    void SetGroupID(size_t newID) { groupID = newID; } 

    Group& operator+=(const Group& other);
};

#endif