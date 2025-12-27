#include "InputHandler.h"
#include "Debug.h"

#ifndef _WIN32
#include <termkey.h>

static TermKey* tk = nullptr;

void InputHandler::Update(float deltaTime)
{
    if (!tk) tk = termkey_new(0, TERMKEY_FLAG_SPACESYMBOL);
    TermKeyKey key;
    while (termkey_getkey_force(tk, &key) == TERMKEY_RES_KEY)
    {
        int code = key.code.codepoint;
        bool isDown = (key.type == TERMKEY_TYPE_KEY);
        auto it = keyBindings.find(code);
        if (it != keyBindings.end())
        {
            it->second(isDown);
        }
    }
}

bool InputHandler::IsKeyDown(int key) const
{
    // libtermkey 没有 IsKeyDown，稍后自己维护状态表
    return false;
}

#endif

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define byte win_byte_override
    #include <windows.h>
    #undef byte

void InputHandler::Update(float deltaTime)
{
    for (auto& kv : keyBindings)
    {
        int vk = kv.first;  // Windows虚拟键码
        SHORT state = GetAsyncKeyState(vk);
        bool isDown = (state & 0x8000) != 0;

        bool wasDown = prevStates[vk];
        if (!wasDown && isDown)
        {
            // Debug::Log("Key pressed: " + std::to_string(vk));
        }

        if (wasDown && !isDown)
        {
            // Debug::Log("Key released: " + std::to_string(vk));
        }

        // ! 注意，这里不管按下与否，都会执行！
        kv.second(isDown);

        prevStates[vk] = isDown;
    }
}

bool InputHandler::IsKeyDown(int key) const
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

#endif