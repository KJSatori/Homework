#ifndef PREFABMANAGER_H
#define PREFABMANAGER_H

#include <unordered_map>
#include <string>
#include <memory>
#include "Prefab.h"
#include "../GameObject/GameObject.h"
#include "../Component/Debug.h"

class PrefabManager
{
private:
    std::unordered_map<std::string, Prefab> prefabs;
    PrefabManager() = default;
public:
    static PrefabManager& Get()
    {
        static PrefabManager inst;
        return inst;
    }

    void Register(const std::string& key, const Prefab& prefab)
    {
        prefabs[key] = prefab;
    }

    bool Has(const std::string& key) const
    {
        return prefabs.find(key) != prefabs.end();
    }

    std::shared_ptr<GameObject> Instantiate(const std::string& key)
    {
        auto it = prefabs.find(key);
        if (it == prefabs.end()) 
        {
            Debug::Log("Prefab " + key + " : 未找到");
            return nullptr;
        }
        return it->second.factory();
    }

    void Clear()
    {
        prefabs.clear();
    }
};

#endif