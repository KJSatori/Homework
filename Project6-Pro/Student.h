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
    //构造/析构/拷贝构造在实现文件中定义，以便做调试计数
    Student(int ID);
    ~Student();
    Student(const Student& other);
    //赋值构造函数
    Student& operator=(const Student& other);

    int GetID() const;
    PlayType MakeDecision(WeatherType weather);
};

#endif