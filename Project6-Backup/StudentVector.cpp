#include "StudentVector.h"

StudentVector::StudentVector(size_t theSize)
{
    elements = new Student*[theSize];
    size = theSize;
}

StudentVector::StudentVector(initializer_list<Student*> students)
{
    elements = new Student*[students.size()];
    size = len = students.size();
    int i = 0;
    for (auto student : students)
    {
        elements[i] = student;
        i++;
    }
}

StudentVector::StudentVector(const StudentVector& other)
{
    len = other.len;
    size = other.size;
    elements = nullptr;
    if (size > 0)
    {
        elements = new Student*[size];
        for (size_t i = 0; i < len; i++) 
            elements[i] = new Student(*(other.elements[i]));
    }
}

StudentVector::StudentVector(StudentVector&& other) noexcept
{
    elements = other.elements;
    len = other.len;
    size = other.size;

    other.elements = nullptr;
    other.len = 0;
    other.size = 0;
}

StudentVector::~StudentVector()
{
    // 删除所有学生对象（如果本容器拥有它们）并释放数组
    if (elements != nullptr)
    {
        for (size_t i = 0; i < len; i++)
        {
            delete elements[i];
        }
        delete[] elements;
        elements = nullptr;
        len = 0;
        size = 0;
    }
}

void StudentVector::DeleteStudents()
{
    for (size_t i = 0; i < len ; i++)
    {
        delete elements[i];
        elements[i] = nullptr;
    }
    len = 0;
}

void StudentVector::Resize()
{
    if (len < size) return;

    // 处理容器为空的情况
    size_t newSize = (size == 0) ? 1 : size * 2;

    // try-catch 语句处理内存分配失败的情况
    // 虽然我觉得这不太可能，但作为学习我还是加上了
    Student** newElements = nullptr;
    try
    {
        newElements = new Student*[newSize];

        for (size_t i = 0; i < len; i++)
        {
            newElements[i] = elements[i];
        }
    }
    catch (const bad_alloc& error)
    {
        delete[] newElements; 
        throw;
    }

    delete[] elements;
    size = newSize;
    elements = newElements;
}

void StudentVector::PushBack(Student* student)
{
    Resize();
    // cout << "在 " << len << " Push Back ID: " << student->GetID() << endl;
    elements[len] = student;
    len++;
}

Student*& StudentVector::operator[](int i)
{
    // 如果索引大于size就抛出错误，防止自己唐
    if (i >= size || i + size < 0) throw out_of_range("Index out of range");
    if (i >= len || i + len < 0) len++;

    if (i >= 0) return elements[i];
    // 支持从后往前索引
    return elements[(int)len + i];
}

Student* StudentVector::operator[](int i) const
{
    // 如果索引大于size就抛出错误，防止自己唐
    if (i >= size || i + size < 0) throw out_of_range("Index out of range");

    if (i >= 0) return elements[i];
    // 支持从后往前索引
    return elements[(int)len + i];
}

StudentVector& StudentVector::operator=(const StudentVector& other)
{
    if (this == &other) return *this;

    // 清理当前内容
    if (elements != nullptr)
    {
        for (size_t i = 0; i < len; i++) delete elements[i];
        delete[] elements;
    }

    len = other.len;
    size = other.size;
    elements = nullptr;
    if (size > 0)
    {
        elements = new Student*[size];
        for (size_t i = 0; i < len; i++)
            elements[i] = new Student(*(other.elements[i]));
        // initialize remaining slots
        for (size_t i = len; i < size; i++) elements[i] = nullptr;
    }

    return *this;
}

StudentVector& StudentVector::operator=(StudentVector&& other) noexcept
{
    if (this == &other) return *this;

    // delete current
    if (elements != nullptr)
    {
        for (size_t i = 0; i < len; i++) delete elements[i];
        delete[] elements;
    }

    // steal
    elements = other.elements;
    len = other.len;
    size = other.size;

    other.elements = nullptr;
    other.len = 0;
    other.size = 0;

    return *this;
}

ostream& operator<< (ostream& os, const StudentVector& studentVector)
{
    for (size_t i = 0; i < studentVector.len; i++)
    {
        os << *(studentVector.elements[i]) << " ";
    }
    return os;
}