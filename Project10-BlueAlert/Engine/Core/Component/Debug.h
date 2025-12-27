#ifndef DEBUG_H
#define DEBUG_H

#include "../../Math/Vector2.h"
#include <string>
#include <vector>
#include "Component.h"

#include <ncursesw/ncurses.h>

using namespace std;

struct Debug : public Component
{
    // 单例
    static Debug* instance;
    WINDOW* win;
    vector<string> messages;
    int maxLines;

    Debug(GameObject* owner = nullptr) : Component(owner), win(nullptr), maxLines(0) {}

    void Awake() override 
    {
        if (!instance) instance = this;
    }

    void Start() override
    {
        Init();
    }

    void Init(int height = 5, int width = COLS, int starty = LINES-5, int startx = 0)
    {
        win = newwin(height, width, starty, startx);
        scrollok(win, TRUE);    // 允许窗口滚动
        maxLines = height;
    }

    void Render(Renderer& renderer) override
    {
        // 每帧刷新窗口
        werase(win);
        for (size_t i = 0; i < messages.size(); ++i)
        {
            mvwprintw(win, i, 0, "%s", messages[i].c_str());
        }
        wrefresh(win);
    }

    static void Log(const Vector2& pos, const std::string& msg)
    {
        mvprintw(pos.y, pos.x, "%s", msg.c_str());
        clrtoeol();
        refresh();
    }

    static void Log(const std::string& msg)
    {
        if (!instance) return;
        instance->messages.push_back(msg);
        if ((int)instance->messages.size() > instance->maxLines)
        {
            instance->messages.erase(instance->messages.begin());
        }
    }

    void Destroy() override
    {
        if (win) delwin(win);
        win = nullptr;
    }
};

#endif