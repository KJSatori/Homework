#include "Student.h"

Student& Student::operator=(const Student& other)
{
    if(this != &other)
    {
        ID = other.ID;

        //重要！删除指针所指内容避免泄露
        delete decision;
        decision = new Decision(*other.decision);
    }
    return *this;
}

int Student::GetID() const
{
    return ID;
}

PlayType Student::MakeDecision(WeatherType weather)
{
    //等价于 (*decision).MakeDecision(weather);
    return decision->MakeDecision(weather);
}

ostream& operator<<(ostream& os, const Student& student)
{
    os << "Stu"<< student.GetID() << ": " << student.decision->GetPercentage();
    return os;
}