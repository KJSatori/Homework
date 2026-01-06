#ifndef DEBUG_H
#define DEBUG_H

#include "../../Math/Vector2.h"
#include <string>
#include <vector>
#include "Component.h"

#include <iostream>

struct Debug : public Component
{
    // 单例
    static Debug* instance;
    std::vector<std::string> messages;
    int maxLines = 10;

    Debug(GameObject* owner = nullptr) : Component(owner) { }
    ~Debug() override;

    void Awake() override 
    {
        if (instance == nullptr) instance = this;
    }

    void Start() override { }

    void Render(Renderer& renderer) override
    {
        // for (auto &m : messages)
        // {
        //     std::cout << m << std::endl;
        //}
    }

    static void Log(const std::string& msg)
    {
        if (!instance) 
        { 
            std::cout << msg << std::endl; 
            return;
        }
        std::cout << msg << std::endl; 
        instance->messages.push_back(msg);
        if ((int)instance->messages.size() > instance->maxLines) 
        {
            instance->messages.erase(instance->messages.begin());
        }
    }
};

#endif