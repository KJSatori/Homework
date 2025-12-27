#ifndef ICOLLIDERLISTENER_H
#define ICOLLIDERLISTENER_H

#include "../GameObject/GameObject.h"
struct IColliderListener
{
    virtual void OnColliderEnter(GameObject* other) {}
    virtual void OnColliderStay(GameObject* other) {}
    virtual void OnColliderExit(GameObject* other) {}
};

#endif