#ifndef STUDENTVECTOR_H
#define STUDENTVECTOR_H

#include "Student.h"
#include <iostream>
#include <initializer_list>

using namespace std;

class StudentVector
{
    friend ostream& operator<< (ostream& os, const StudentVector& studentVector);

private:
    Student** elements = nullptr;
    size_t len = 0;
    size_t size = 0;
    void Resize();

public:
    StudentVector() = default;
    StudentVector(size_t theSize);
    StudentVector(initializer_list<Student*> students);
    StudentVector(const StudentVector& other);
    StudentVector(StudentVector&& other) noexcept;
    ~StudentVector();

    size_t Capacity() const { return size; }
    size_t Length() const { return len; } 
    bool Empty() const { return len == 0; }
    void PushBack(Student* student);

    void DeleteStudents();
    // 从当前向量中提取 [start, start+count) 范围的元素，转移其所有权到返回向量
    StudentVector ExtractRange(size_t start, size_t count);

    Student*& operator[](int i);
    Student* operator[](int i) const;
    StudentVector& operator=(const StudentVector& other);
    StudentVector& operator=(StudentVector&& other) noexcept;
};

#endif