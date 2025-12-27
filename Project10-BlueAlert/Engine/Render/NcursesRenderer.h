#ifndef NCURSES_RENDERER_H
#define NCURSES_RENDERER_H

#include <ncursesw/ncurses.h>
#include <locale.h>
#include "Renderer.h"
#include "TerminalMapper.h"

struct NcursesRenderer : public Renderer {
    NcursesRenderer() {
        setlocale(LC_ALL, "");     // 启用 UTF-8 本地化
        initscr();                 // 初始化 ncurses
        noecho();                  // 不回显输入
        curs_set(0);               // 隐藏光标
        nodelay(stdscr, TRUE);

        xScale = 0.5f;   // 逻辑x长度缩放为原来的一半
        yScale = 1.0f;
    }

    ~NcursesRenderer() {
        endwin();                  // 结束 ncurses
    }

    void DrawPoint(const Vector2& pos, char ch, int colorPair) override {
        Vector2 termPos = TerminalMapper::ToTerminalCoordinates(pos);
        mvaddch((int)termPos.y, (int)termPos.x, ch);
    }

    /// @brief 会将逻辑x坐标乘以2渲染到终端
    void DrawWidePoint(const Vector2& pos, wchar_t ch, int colorPair = 1) override {
        Vector2 termPos = TerminalMapper::ToTerminalCoordinates(pos);

        cchar_t wch;
        wchar_t wstr[2] = { ch, L'\0' };
        setcchar(&wch, wstr, 0, 0, NULL);

        attron(COLOR_PAIR(colorPair));
        mvadd_wch((int)termPos.y, (int)termPos.x, &wch);
        attroff(COLOR_PAIR(colorPair));
    }

    void DrawMsg(const Vector2& pos, const wchar_t* msg) override {
        Vector2 termPos = TerminalMapper::ToTerminalCoordinates(pos);
        mvaddwstr((int)termPos.y, (int)termPos.x, msg);
    }

    void Clear() override {
        ::clear();
    }

    void Erase() override {
        ::erase();
    }

    void Refresh() override {
        ::refresh();
    }
};

#endif