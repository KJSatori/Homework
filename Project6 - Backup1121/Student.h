#ifndef STUDENT_H
#define STUDENT_H
#include "Decision.h"
#include "Utils.h"
#include "Weather.h"
#include <iostream>
using namespace std;

class Student
{
    friend ostream& operator<<(ostream& os, const Student& student);

private:
    int ID;
    Decision* decision;

public:
    //构造函数
    Student(int ID): ID(ID), decision(new Decision(GenerateRandf())) {}
    //析构函数
    ~Student() { delete decision; }
    //拷贝构造函数
    Student(const Student& other): ID(other.ID) 
    {
        decision = new Decision(*other.decision); 
    }
    //赋值构造函数
    Student& operator=(const Student& other);

    int GetID() const;
    PlayType MakeDecision(WeatherType weather);
};

#endif