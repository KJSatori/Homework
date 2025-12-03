#include "BigInt.h"

vector<int> BigInt::ConvertToVector(int num)
{
    vector<int> temp;
    while (num / 10 != 0)
    {
        temp.push_back(num % 10);
        num /= 10;
    }
    temp.push_back(num);
    return temp;
}

void BigInt::Formalize()
{
    while (digits.size() > 1 && digits.back() == 0)
    {
        digits.pop_back();
    }
}

BigInt::BigInt(int num)
{
    digits = ConvertToVector(num);
}

BigInt::BigInt(const string &str)
{
    for (int i = str.length() - 1; i >= 0; i--)
    {
        digits.push_back(str[i] - 48);
    }
    Formalize();
}

BigInt::BigInt(const char *str)
{
    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        digits.push_back(str[i] - 48);
    }
    Formalize();
}

bool BigInt::isZero() const
{
    return (digits.size() == 1 && digits[0] == 0) || digits.empty();
}

BigInt BigInt::operator+(const BigInt &other) const
{
    BigInt result;
    int i = 0;
    int carry = 0;
    int digit = 0;
    for (; i < other.digits.size() && i < digits.size(); i++)
    {
        digit = other.digits[i] + digits[i] + carry;
        carry = digit / 10;
        digit %= 10;
        result.digits.push_back(digit);
    }

    for (; i < other.digits.size(); i++)
    {
        digit = carry + other.digits[i];
        carry = digit / 10;
        digit %= 10;
        result.digits.push_back(digit);
    }
    for (; i < digits.size(); i++)
    {
        digit = carry + digits[i];
        carry = digit / 10;
        digit %= 10;
        result.digits.push_back(digit);
    }
    if (carry != 0) result.digits.push_back(carry);
    result.Formalize();
    return result;
}

BigInt &BigInt::operator+=(const BigInt &other)
{
    *this = *this + other;
    Formalize();
    return *this;
}

bool BigInt::operator==(const BigInt &other) const
{
    return digits == other.digits;
}

bool BigInt::operator<(const BigInt &other) const
{
    if (digits.size() != other.digits.size())
        return digits.size() < other.digits.size();

    for (int i = digits.size() - 1; i >= 0; i--)
    {
        if (digits[i] != other.digits[i])
            return digits[i] < other.digits[i];
    }
    return false;
}

bool BigInt::operator<=(const BigInt &other) const
{
    return (*this == other) || (*this < other);
}

bool BigInt::operator>(const BigInt &other) const
{
    return !(*this <= other);
}

bool BigInt::operator>=(const BigInt &other) const
{
    return !(*this < other);
}

bool BigInt::operator!=(const BigInt &other) const
{
    return !(*this == other);
}

BigInt BigInt::operator*(const BigInt &other) const
{
    BigInt result;
    for (int i = 0; i < other.digits.size(); i++)
    {
        int carry = 0;
        int mulitplyDigit = other.digits[i];
        BigInt singleDigitResult;
        for (int j = 0; j < i; j++)
        {
            singleDigitResult.digits.push_back(0);
        }
        for (int j = 0; j < digits.size(); j++)
        {
            int digit = digits[j] * mulitplyDigit + carry;
            carry = digit / 10;
            digit %= 10;
            singleDigitResult.digits.push_back(digit);
        }
        if (carry != 0) singleDigitResult.digits.push_back(carry);
        result += singleDigitResult;
    }
    result.Formalize();
    return result;
}

BigInt &BigInt::operator*=(const BigInt &other)
{
    *this = *this * other;
    Formalize();
    return *this;
}

ostream &operator<<(ostream &os, const BigInt& bigInt)
{
    if (bigInt.isZero())
    {
        os << 0;
        return os;
    }
    for (auto iter = bigInt.digits.rbegin(); iter != bigInt.digits.rend(); iter++)
    {
        os << *iter;
    }
    return os;
}

istream &operator>>(istream &is, BigInt &bigInt)
{
    string temp;
    is >> temp;
    bigInt = BigInt(temp);
    bigInt.Formalize();
    return is;
}
