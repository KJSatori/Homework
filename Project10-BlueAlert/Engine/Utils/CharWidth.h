#ifndef CHARWIDTH_H
#define CHARWIDTH_H

#include <cwchar>
#include <locale>

inline int GetCharWidth(wchar_t ch)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows 下没有 wcwidth
    if (ch >= 0x20 && ch <= 0x7E) return 1; // ASCII

    if ((ch >= 0x4E00 && ch <= 0x9FFF) ||   // 中日韩文字
        (ch >= 0x3000 && ch <= 0x303F) ||   // 全角标点
        (ch >= 0xFF00 && ch <= 0xFFEF))     // 全角ASCII
        return 2;
    return 1;
#else
    // Linux/Unix 下直接调用 wcwidth
    int w = wcwidth(ch);
    return (w < 0 ? 1 : w);
#endif
}

#endif