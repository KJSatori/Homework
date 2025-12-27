#ifndef SCRIPT_H
#define SCRIPT_H

#include "../Component.h"
#include "../IColliderListener.h"
#include "../../GameObject/GameObject.h"

struct Script : public Component, public IColliderListener
{
    Script (GameObject* owner) : Component(owner) {}

    /// @brief 添加脚本后立刻调用
    virtual void Awake() override {}

    /// @brief 所有脚本与GameObject添加完后调用
    virtual void Start() override {}

    /// @brief 每帧调用
    /// @param deltaTime 帧间时间间隔 
    virtual void Update(float deltaTime) override {}

    /// @brief 每帧之后立马调用
    /// @param renderer 
    virtual void Render(Renderer& renderer) {}

    /// @brief Scene摧毁或者GameObject摧毁时调用
    virtual void Destroy() override {}

    virtual void OnColliderEnter(GameObject* other) override {}
    virtual void OnColliderStay(GameObject* other) override {}
    virtual void OnColliderExit(GameObject* other) override {}
};

#endif