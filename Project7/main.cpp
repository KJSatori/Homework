#include <iostream>
#include "BigInt.h"
using namespace std;

int main() 
{
    // 构造函数测试
    BigInt a(0);
    BigInt b("12345");
    BigInt c("000012345");   // 测试 Formalize 去掉前导零
    BigInt d("54321");
    BigInt e("999999999999999999999999"); // 大数测试

    cout << "构造和输出测试:" << endl;
    cout << "a = " << a << " (期望 0)" << endl;
    cout << "b = " << b << " (期望 12345)" << endl;
    cout << "c = " << c << " (期望 12345)" << endl;
    cout << "d = " << d << " (期望 54321)" << endl;
    cout << "e = " << e << " (期望 999999999999999999999999)" << endl;

    // 比较运算符测试
    cout << "\n比较运算符测试:" << endl;
    cout << "b == c ? " << (b == c) << " (期望 1)" << endl;
    cout << "b != d ? " << (b != d) << " (期望 1)" << endl;
    cout << "b < d ? " << (b < d) << " (期望 1)" << endl;
    cout << "d > b ? " << (d > b) << " (期望 1)" << endl;
    cout << "b <= c ? " << (b <= c) << " (期望 1)" << endl;
    cout << "d >= b ? " << (d >= b) << " (期望 1)" << endl;

    // 加法测试
    cout << "\n加法测试:" << endl;
    BigInt sum1 = b + d;
    cout << "12345 + 54321 = " << sum1 << " (期望 66666)" << endl;

    BigInt sum2 = e + BigInt("1");
    cout << "999999999999999999999999 + 1 = " << sum2 
         << " (期望 1000000000000000000000000)" << endl;

    // 乘法测试
    cout << "\n乘法测试:" << endl;
    BigInt mul1 = b * d;
    cout << "12345 * 54321 = " << mul1 << " (期望 670592745)" << endl;

    BigInt mul2 = BigInt("1000000000000000000") * BigInt("1000000000000000000");
    cout << "10^18 * 10^18 = " << mul2 << " (期望 1000000000000000000000000000000000000)" << endl;

    // 复合赋值测试
    cout << "\n复合赋值测试:" << endl;
    BigInt f("100");
    f += BigInt("23");
    cout << "100 += 23 -> " << f << " (期望 123)" << endl;

    BigInt g("12");
    g *= BigInt("34");
    cout << "12 *= 34 -> " << g << " (期望 408)" << endl;

    // 输入测试
    cout << "\n输入测试 (请输入一个大整数): ";
    BigInt h;
    cin >> h;
    cout << "你输入的是: " << h << endl;

    return 0;
}