#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "Component.h"
#include <unordered_map>
#include <functional>
#include "../GameObject/GameObject.h"
#include "../../Render/Renderer.h"
#include "../../Math/Vector2.h"
#include "Rigidbody.h"
#include "Debug.h"

#include <ncursesw/ncurses.h>

#ifndef KEYCODE_H
#define KEYCODE_H

#ifdef _WIN32
    // Windows 下字母键直接用 ASCII
    #define KEY_W 'W'
    #define KEY_A 'A'
    #define KEY_S 'S'
    #define KEY_D 'D'
    #define KEY_SPACE VK_SPACE
#else
    // Linux 下 ncurses getch() 返回小写字母
    #define KEY_W 'w'
    #define KEY_A 'a'
    #define KEY_S 's'
    #define KEY_D 'd'
    #define KEY_SPACE ' '
    #define KEY_ENTER 10
#endif

#endif

struct InputHandler : public Component
{
    unordered_map<int, bool> prevStates;

    /// @brief 这里void(bool)的bool 表示 KEY_DOWN(bool) 与 KEY_UP(!bool)
    unordered_map<int, function<void(bool)>> keyBindings;

    InputHandler(GameObject* owner) : Component(owner) {}

    ///@brief 注册按键绑定
    void BindKey(int key, function<void(bool)> action)
    {
        keyBindings[key] = action;
        prevStates[key] = false;
    }

    void Update(float deltaTime) override;

    bool IsKeyDown(int key) const;
};


#endif