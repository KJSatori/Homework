#include "Student.h"
#include <atomic>

static std::atomic<int> g_student_allocs{0};
static std::atomic<int> g_student_frees{0};

void Student::PrintAllocStats()
{
    std::cout << "Student allocs: " << g_student_allocs.load()
              << ", frees: " << g_student_frees.load() << std::endl;
}

Student::Student(int ID): ID(ID), decision(new Decision(GenerateRandf()))
{
    ++g_student_allocs;
}

Student::~Student()
{
    delete decision;
    ++g_student_frees;
}

Student::Student(const Student& other): ID(other.ID)
{
    decision = new Decision(*other.decision);
    ++g_student_allocs;
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