#include "StudentVector.h"

StudentVector::StudentVector(size_t theSize)
{
    elements = new Student*[theSize];
    size = theSize;
    // 初始化为 nullptr，防止未初始化内存导致不可预测行为
    for (size_t i = 0; i < size; ++i) elements[i] = nullptr;
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
        // 初始化剩余槽为 nullptr，防止析构时删除未初始化指针
        for (size_t i = len; i < size; ++i) elements[i] = nullptr;
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
        // 初始化剩余槽为 nullptr
        for (size_t i = len; i < newSize; ++i) newElements[i] = nullptr;
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

StudentVector StudentVector::ExtractRange(size_t start, size_t count)
{
    if (start + count > len) throw out_of_range("ExtractRange out of range");

    StudentVector out(count);
    out.len = count;
    for (size_t i = 0; i < count; i++)
    {
        out.elements[i] = elements[start + i];
        elements[start + i] = nullptr; // 转移所有权
    }

    // 调整当前向量长度为 start（即移除提取的部分）
    len = start;

    return out;
}

Student*& StudentVector::operator[](int i)
{
    // 索引必须在已有长度范围内（不自动扩容）
    if (i < 0)
    {
        int absIndex = (int)len + i; // i negative
        if (absIndex < 0) throw out_of_range("Index out of range");
        return elements[absIndex];
    }
    // 正向索引
    if ((size_t)i >= len) throw out_of_range("Index out of range");
    return elements[i];
}

Student* StudentVector::operator[](int i) const
{
    if (i < 0)
    {
        int absIndex = (int)len + i;
        if (absIndex < 0) throw out_of_range("Index out of range");
        return elements[absIndex];
    }
    if ((size_t)i >= len) throw out_of_range("Index out of range");
    return elements[i];
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