#include "Student.h"

Student::Student(int ID): ID(ID), decision(new Decision(GenerateRandf()))
{

}

Student::~Student()
{
    delete decision;

}

Student::Student(const Student& other): ID(other.ID)
{
    decision = new Decision(*other.decision);
}

Student& Student::operator=(const Student& other)
{
    if (this != &other)
    {
        ID = other.ID;
        delete decision;
        decision = new Decision(*other.decision);
    }
    return *this;
}

PlayType Student::MakeDecision(WeatherType weather)
{
    return decision->MakeDecision(weather);
}

int Student::GetID() const
{
    return ID;
}

ostream& operator<<(ostream& os, const Student& student)
{
    os << "Stu" << student.GetID() << ": " << student.decision->GetPercentage();
    return os;
}