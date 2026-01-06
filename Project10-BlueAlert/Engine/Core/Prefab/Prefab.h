#ifndef PREFAB_H
#define PREFAB_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "../GameObject/GameObject.h"

struct Prefab
{
    std::string key;
    std::function<std::shared_ptr<GameObject>(void)> factory;
};

#endif