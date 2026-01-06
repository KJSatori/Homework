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

struct InputHandler : public Component
{
    unordered_map<int, bool> prevStates;

    struct KeyBindings
    {
        function<void()> onDown;
        function<void()> onUp;
    };
    
    unordered_map<int, KeyBindings> keyBindings;

    InputHandler(GameObject* owner) : Component(owner) {}

    ///@brief 注册按键绑定
    void BindKey(int key, function<void()> downAction, function<void()> upAction)
    {
        keyBindings[key].onDown = downAction;
        keyBindings[key].onUp   = upAction;
        prevStates[key]         = false;
    }

    void Update(float deltaTime) override;

    void ProcessEvent(const SDL_Event& e) override;

    bool IsKeyDown(int key) const;
};


#endif