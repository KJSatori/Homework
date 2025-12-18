#ifndef _GUI_H_
#define _GUI_H_
#include <ncurses.h>

class Gui {
    size_t row;
    size_t col;
    WINDOW* win;
public:
    Gui ():row(20),col(50) {Init();}
    ~Gui() {endwin();}
    void Init();
    int ReadInput();
    void DrawCharAt(size_t, size_t, char, int colorPair = 0);
    void DrawBoldCharAt(size_t, size_t, char, int colorPair = 0);
    void DrawTextAt(int row, int col, const char* prompt);
    void ClearWindow();
    void RefreshWindow();
    void End() {endwin();}
};
#endif
