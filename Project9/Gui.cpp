#include <ncurses.h>
#include "Gui.h"
#include <cstring>
using namespace std;

void Gui::Init() {
    initscr();                  /* Start curses mode                */
    if (has_colors())
    {
        start_color();
        use_default_colors();
        // 定义颜色对，因为这是小作业 我就随便在这里定义了
        init_pair(1, COLOR_GREEN, COLOR_BLACK);  // Grass
        init_pair(2, COLOR_WHITE, COLOR_BLACK);  // Rabbit
        init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Wolf
        init_pair(4, COLOR_RED, COLOR_BLACK);    // Tiger
    }
    refresh();                  /* Print it on to the real screen   */
    win = stdscr;
    refresh();
    curs_set(0);
    wrefresh(win);
    noecho();
    cbreak();
    keypad(win, true);
    nodelay(win, true);
    flushinp();
}

/// @brief 从窗口获得单个字符（非阻塞输入）
/// @return 
int Gui::ReadInput() {
    int c = wgetch(win);
    flushinp();
    return c;
}

/// @brief 在指定位置写入单个字符
/// @param row 列
/// @param col 行
/// @param c 字符
void Gui::DrawCharAt(size_t row, size_t col, char c, int colorPair) {
    if (colorPair != 0 && has_colors())
    {
        chtype ch = static_cast<chtype>(c) | COLOR_PAIR(colorPair);
        mvwaddch(win, row, col, ch);
    }
    else mvwaddch(win, row, col, c);
    return;
}

void Gui::DrawBoldCharAt(size_t row, size_t col, char c, int colorPair)
{if (colorPair != 0 && has_colors())
    {
        chtype ch = static_cast<chtype>(c) | COLOR_PAIR(colorPair) | A_BOLD;
        mvwaddch(win, row, col, ch);
    }
    else mvwaddch(win, row, col, c);
    return;
}

/// @brief 在指定位置写入字符串
/// @param row 列
/// @param col 行
/// @param prompt C风格字符串
void Gui::DrawTextAt(int row, int col, const char* prompt) {
    mvwprintw(win, row, col, prompt);
}

/// @brief 清除整个窗口（用空格填充整个窗口）
void Gui::ClearWindow() {
    werase(win);
}

/// @brief 将窗口缓冲区刷新到屏幕
void Gui::RefreshWindow() {
    wrefresh(win);
}
