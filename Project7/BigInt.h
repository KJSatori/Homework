#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
using namespace std;

class BigInt
{
private:
    // I've already learned C++, so I don't want to use arrays anymore!
    // It just brings some samely extra works~
    // Same with project6.
    vector<int> digits;

    vector<int> ConvertToVector(int num);
    void Formalize();

public:
    BigInt(): digits(vector<int>()) {}
    // Since I used vector, it will deep copy automatically.
    BigInt(const BigInt& other) = default;
    BigInt(int num);
    BigInt(const string& str);
    BigInt(const char* str);

    // Since I used vector, destructor is unnecessary. 
    ~BigInt() = default;

    bool isZero() const;

    BigInt& operator=(const BigInt& other) = default;
    BigInt operator+(const BigInt& other) const;
    BigInt& operator+=(const BigInt& other);
    bool operator==(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt& operator*=(const BigInt& other);

    friend ostream& operator<<(ostream& os, const BigInt& bigInt);
    friend istream& operator>>(istream& is, BigInt& bigInt);
};

#endif