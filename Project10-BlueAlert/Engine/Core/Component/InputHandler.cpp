#include "InputHandler.h"
#include "Debug.h"

void InputHandler::Update(float deltaTime)
{
    // 本帧按键状态
    unordered_map<int, bool> currStates = prevStates;

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            Debug::Log("SDL_QUIT received");
            break;
        
        case SDL_KEYDOWN:
        {
            if (e.key.repeat) break;
            int code = e.key.keysym.sym;
            currStates[code] = true;

            auto it = keyBindings.find(code);
            bool wasDown = prevStates[code];

            if (it != keyBindings.end() && !wasDown)
            {
                it->second.onDown();
            }
            break;
        }
        case SDL_KEYUP:
        {
            int code = e.key.keysym.sym;
            currStates[code] = false;

            auto it = keyBindings.find(code);
            bool wasDown = prevStates[code];
            if (it != keyBindings.end() && wasDown)
            {
                it->second.onUp();
            }
            break;
        }
        default:
            break;
        }
    }
    prevStates = currStates;
}

void InputHandler::ProcessEvent(const SDL_Event &e)
{
    // Debug::Log("分发了事件: " + to_string(e.type));
    switch (e.type)
    {
    case SDL_KEYDOWN:
        if (e.key.repeat) break;
        {
            int code = e.key.keysym.sym;
            bool wasDown = prevStates[code];
            prevStates[code] = true;

            auto it = keyBindings.find(code);
            if (it != keyBindings.end() && !wasDown)
            {
                it->second.onDown();
            }
        }
        break;
    case SDL_KEYUP:
        {
            int code = e.key.keysym.sym;
            bool wasDown = prevStates[code];
            prevStates[code] = false;

            auto it = keyBindings.find(code);
            if (it != keyBindings.end() && wasDown)
            {
                it->second.onUp();
            }
        }
        break;
    default:
        break;
    }
}

bool InputHandler::IsKeyDown(int key) const 
{ 
    auto it = prevStates.find(key);
    return it != prevStates.end() && it->second; 
}