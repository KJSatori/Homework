#ifndef COMPONENT_H
#define COMPONENT_H

struct Renderer;
struct GameObject;

struct Component
{
    // 用原始指针避免循环，而不需要在乎gameObject的生命周期
    GameObject* gameObject;
    
    Component(GameObject* owner = nullptr) : gameObject(owner) {}

    virtual void Awake() {}
    virtual void Start() {}
    virtual void Update(float deltaTime) {}
    virtual void Render(struct Renderer& renderer) {}
    virtual void Destroy() {}
    virtual ~Component() {}
};


#endif