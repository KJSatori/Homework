#include "Scene.h"
#include "../GameObject/GameObject.h"
#include "../Component/IColliderListener.h"
#include "../Component/RendererComponent.h"
#include "../../Math/Utils.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

// 用于空间哈希碰撞检测的单元格大小, 可根据碰撞箱大小自行调整
const float SPATIAL_CELL_SIZE = 48.0f;

// 覆盖格子数量超过该值视为 large object
const int LARGE_OBJECT_CELL_THRESHOLD = 12;

/// @brief  临时用于碰撞检测的 AABB 辅助函数, 稍后迁移, 下同
inline void WorldToCell(const Vector2& pos, int& ix, int &iy, float cellSize)
{
    ix = static_cast<int>(std::floor(pos.x / cellSize));
    iy = static_cast<int>(std::floor(pos.y / cellSize));
}

/// @brief AABB 与 格子索引范围
inline void AABBToCellRange(const AABB& aabb, int& ixMin, int& iyMin, int& ixMax, int& iyMax, float cellSize)
{
    ixMin = static_cast<int>(std::floor(aabb.min.x / cellSize)); 
    iyMin = static_cast<int>(std::floor(aabb.min.y / cellSize)); 
    ixMax = static_cast<int>(std::floor(aabb.max.x / cellSize)); 
    iyMax = static_cast<int>(std::floor(aabb.max.y / cellSize));
}

// AABB 重叠快速检测 
inline bool AABBOverlap(const AABB& a, const AABB& b) 
{ 
    return !(a.max.x < b.min.x || a.min.x > b.max.x || a.max.y < b.min.y || a.min.y > b.max.y); 
}

// 假设 GameObject.h 已包含 queued 字段
void Scene::AddObject(const std::shared_ptr<GameObject>& object)
{
    if (!object) return;

    /*
    std::cerr << "[AddObject] called: name=" << object->name
              << " addr=" << (uintptr_t)object.get()
              << " use_count=" << object.use_count()
              << " queued=" << (object->queued ? "1":"0")
              << " scene_ptr=" << (uintptr_t)this
              << std::endl;
    */

    if (object->GetSceneNoWarn() == this) {
        std::cerr << "[AddObject] -> already in this scene: " << object->name << std::endl;
        return;
    }

    if (object->queued) {
        std::cerr << "[AddObject] -> already pending (queued flag): " << object->name << std::endl;
        return;
    }

    object->SetScene(this);

    object->Awake();

    object->queued = true;
    toAdd.push_back(object);

    /*
    std::cerr << "[AddObject] -> queued: " << object->name
              << " toAdd.size=" << toAdd.size() << std::endl;
    */
}

void Scene::Start()
{
    isStarted = true;

    if (!toAdd.empty()) {
        std::vector<std::shared_ptr<GameObject>> temp;
        temp.swap(toAdd);
        objects.reserve(objects.size() + temp.size());
        for (auto& o : temp) {
            if (!o) continue;
            o->Start();
            objects.push_back(o);
        }
    }
}

void Scene::Update(float deltaTime)
{        
    for (auto& obj : objects)
    {
        // 确保 父的 children 不包含已经销毁的子物体
        if (obj) obj->CleanUp();
    }

    // 防御性编程
    objects.erase(
        remove_if(objects.begin(), objects.end(),
        [](const shared_ptr<GameObject>& o) { return !o || o->isDestroyed; }),
        objects.end()
    );

    // std::cerr << "[Update] Before merge: toAdd.size=" << toAdd.size()
    //           << " toAdd.data=" << (uintptr_t)toAdd.data()
    //           << " objects.size=" << objects.size()
    //           << " objects.data=" << (uintptr_t)objects.data()
    //           << std::endl;

    const size_t MAX_MERGE_ITER = 1000;
    size_t mergeCount = 0;
    while (!toAdd.empty() && mergeCount < MAX_MERGE_ITER)
    {
        std::vector<std::shared_ptr<GameObject>> temp;
        temp.swap(toAdd);

        /*
        std::cerr << "[Update] Merging batch size=" << temp.size()
                  << " temp.data=" << (uintptr_t)temp.data()
                  << " toAdd.size(after swap)=" << toAdd.size()
                  << std::endl;
        */
        objects.reserve(objects.size() + temp.size());

        std::unordered_set<uintptr_t> seen;

        for (auto& o : temp)
        {
            if (!o) continue;

            if (isStarted) o->Start();

            uintptr_t addr = (uintptr_t)o.get();

            /*
            std::cerr << "[Update] Processing temp item: name=" << o->name
                      << " addr=" << addr
                      << " use_count=" << o.use_count()
                      << std::endl;
            */

            if (seen.count(addr) ||
                std::any_of(objects.begin(), objects.end(),
                    [&](const std::shared_ptr<GameObject>& ex){ return ex.get() == o.get(); }))
            {
                std::cerr << "[Update] Skip duplicate or already-in-objects: " << o->name
                          << " addr=" << addr << std::endl;
                o->queued = false; 
                continue;
            }

            objects.push_back(o);

            o->queued = false; 
            seen.insert(addr);

            /*
            std::cerr << "[Update] Merged into objects: " << o->name
                      << " addr=" << addr
                      << " objects.size=" << objects.size()
                      << std::endl;
            */
        }

        mergeCount++;
    }

    if (mergeCount >= MAX_MERGE_ITER) {
        std::cerr << "[Update] WARNING: merge reached MAX_MERGE_ITER, possible infinite add loop" << std::endl;
    }

    // 遍历前将需要添加的物体添加到 objects
    if (!toAdd.empty())
    {
        // 提前分配容器大小减少开销
        objects.reserve(objects.size() + toAdd.size());
        for (auto& o : toAdd)
        {
            Debug::Log("添加到objects: " + o->name);
            if (o) objects.push_back(o);
        }
        toAdd.clear();
    }

    for (auto& obj : objects) 
    {
        obj->Update(deltaTime);
    }

    // 构建扁平对象，包含所有活跃子物体
    // --- begin spatial hash collision block ---
    vector<shared_ptr<GameObject>> flatObjects;
    flatObjects.reserve(objects.size() * 2);
    for (auto& root : objects) CollectObjectChildren(root, flatObjects);

    // collect active colliders
    vector<Collider*> colliders;
    colliders.reserve(flatObjects.size());
    for (auto& go : flatObjects) {
        if (!go || go->isDestroyed || !go->IsActive()) continue;
        auto col = go->GetComponent<Collider>();
        if (!col) continue;
        colliders.push_back(col.get());
    }

    // spatial hash containers
    std::unordered_map<int64_t, std::vector<Collider*>> grid;
    grid.reserve(colliders.size() * 2);
    std::vector<Collider*> largeObjects;
    largeObjects.reserve(64);

    // insert colliders into grid or largeObjects
    for (auto* col : colliders) {
        // layer/mask quick filter could be applied here if you have global rules
        AABB aabb = col->GetAABB(); // 需要 Collider 提供
        int ixMin, iyMin, ixMax, iyMax;
        AABBToCellRange(aabb, ixMin, iyMin, ixMax, iyMax, SPATIAL_CELL_SIZE);
        int covered = (ixMax - ixMin + 1) * (iyMax - iyMin + 1);
        if (covered <= LARGE_OBJECT_CELL_THRESHOLD) {
            for (int ix = ixMin; ix <= ixMax; ++ix) {
                for (int iy = iyMin; iy <= iyMax; ++iy) {
                    grid[CellKey(ix, iy)].push_back(col);
                }
            }
        } else {
            largeObjects.push_back(col);
        }
    }

    // helper to push collision pair (ensures ordering a<b)
    auto pushPair = [&](Collider* a, Collider* b, vector<CollisionPair>& out) {
        if (a->gameObject == b->gameObject) return;
        // ensure deterministic ordering to avoid duplicates
        if (a->gameObject < b->gameObject) out.push_back({ a->gameObject, b->gameObject });
        else out.push_back({ b->gameObject, a->gameObject });
    };

    // process grid cells (cell-local candidate pairs)
    vector<CollisionPair> nextActiceCollisionsLocal;
    nextActiceCollisionsLocal.reserve(256);
    for (auto& kv : grid) {
        auto& list = kv.second;
        size_t n = list.size();
        for (size_t i = 0; i < n; ++i) {
            Collider* A = list[i];
            AABB aA = A->GetAABB();
            for (size_t j = i + 1; j < n; ++j) {
                Collider* B = list[j];
                // layer/mask filter (if available)
                if (!A->CanCollideWith(*B)) continue; // 假设 Collider 提供 CanCollideWith
                AABB aB = B->GetAABB();
                if (!AABBOverlap(aA, aB)) continue;
                // narrow-phase
                if (A->Intersects(*B)) pushPair(A, B, nextActiceCollisionsLocal);
            }
        }
    }

    // process largeObjects vs grid objects (avoid duplicate checks using seen set per large)
    for (auto* L : largeObjects) {
        AABB aL = L->GetAABB();
        int ixMin, iyMin, ixMax, iyMax;
        AABBToCellRange(aL, ixMin, iyMin, ixMax, iyMax, SPATIAL_CELL_SIZE);
        std::unordered_set<uintptr_t> seen; // avoid duplicate small checks
        for (int ix = ixMin; ix <= ixMax; ++ix) {
            for (int iy = iyMin; iy <= iyMax; ++iy) {
                auto it = grid.find(CellKey(ix, iy));
                if (it == grid.end()) continue;
                for (auto* S : it->second) {
                    uintptr_t key = (uintptr_t)S;
                    if (seen.count(key)) continue;
                    seen.insert(key);
                    if (!L->CanCollideWith(*S)) continue;
                    AABB aS = S->GetAABB();
                    if (!AABBOverlap(aL, aS)) continue;
                    if (L->Intersects(*S)) pushPair(L, S, nextActiceCollisionsLocal);
                }
            }
        }
    }

    // process large vs large
    for (size_t i = 0; i < largeObjects.size(); ++i) {
        for (size_t j = i + 1; j < largeObjects.size(); ++j) {
            Collider* A = largeObjects[i];
            Collider* B = largeObjects[j];
            if (!A->CanCollideWith(*B)) continue;
            if (!AABBOverlap(A->GetAABB(), B->GetAABB())) continue;
            if (A->Intersects(*B)) pushPair(A, B, nextActiceCollisionsLocal);
        }
    }

    // deduplicate nextActiceCollisionsLocal (optional if pushPair ordering used)
    std::sort(nextActiceCollisionsLocal.begin(), nextActiceCollisionsLocal.end(),
        [](const CollisionPair& a, const CollisionPair& b) {
            if (a.a != b.a) return a.a < b.a;
            return a.b < b.b;
        });
    nextActiceCollisionsLocal.erase(
        std::unique(nextActiceCollisionsLocal.begin(), nextActiceCollisionsLocal.end(),
            [](const CollisionPair& x, const CollisionPair& y){ return x.a==y.a && x.b==y.b; }),
        nextActiceCollisionsLocal.end()
    );

    // now nextActiceCollisionsLocal contains candidate collision pairs
    // convert to your existing collision enter/stay/exit logic
    for (const auto& newColPair : nextActiceCollisionsLocal) {
        auto iter = find(activeCollisions.begin(), activeCollisions.end(), newColPair);
        auto colListenerA = newColPair.a->GetComponents<IColliderListener>();
        auto colListenerB = newColPair.b->GetComponents<IColliderListener>();
        if (iter == activeCollisions.end()) {
            for (auto& comp : colListenerA) if (comp) comp->OnColliderEnter(newColPair.b);
            for (auto& comp : colListenerB) if (comp) comp->OnColliderEnter(newColPair.a);
        } else {
            for (auto& comp : colListenerA) if (comp) comp->OnColliderStay(newColPair.b);
            for (auto& comp : colListenerB) if (comp) comp->OnColliderStay(newColPair.a);
        }
        // collect into nextActiceCollisions for later exit detection
        // (we'll assign activeCollisions = move(nextActiceCollisionsLocal) later)
    }

    // replace activeCollisions assignment with the local vector
    vector<CollisionPair> nextActiceCollisions = std::move(nextActiceCollisionsLocal);
    // --- end spatial hash collision block ---

    /*
    vector<CollisionPair> nextActiceCollisions;

    for (size_t i = 0; i < flatObjects.size(); ++i)
    {
        auto a = flatObjects[i];
        if (!a || a->isDestroyed || !(a->IsActive())) continue;
        auto colA = a->GetComponent<Collider>();
        if (!colA) continue;

        for (size_t j = i + 1; j < flatObjects.size(); ++j)
        {
            auto b = flatObjects[j];
            if (!b || b->isDestroyed || !(b->IsActive())) continue;
            auto colB = b->GetComponent<Collider>();
            if (!colB) continue;

            if (!colA->Intersects(*colB)) continue;
            CollisionPair newColPair{ colA->gameObject, colB->gameObject };
            auto iter = find(activeCollisions.begin(), activeCollisions.end(), newColPair);

            auto colListenerA = newColPair.a->GetComponents<IColliderListener>();
            auto colListenerB = newColPair.b->GetComponents<IColliderListener>();
            if (iter == activeCollisions.end())
            {
                // 调用 A 与 B 的 OnCollisionEnter;
                for (auto& comp : colListenerA) if (comp) comp->OnColliderEnter(newColPair.b); 
                for (auto& comp : colListenerB) if (comp) comp->OnColliderEnter(newColPair.a);

                nextActiceCollisions.push_back(newColPair);
            }
            else
            {
                // 调用 A 与 B 的 OnCollisionStay;
                for (auto& comp : colListenerA) if (comp) comp->OnColliderStay(newColPair.b); 
                for (auto& comp : colListenerB) if (comp) comp->OnColliderStay(newColPair.a);

                nextActiceCollisions.push_back(newColPair);
            }
        }
    }

    for (const auto& pair : activeCollisions)
    {
        auto iter = find(nextActiceCollisions.begin(), nextActiceCollisions.end(), pair);
        if (iter == nextActiceCollisions.end())
        {
            auto colListenerA = pair.a->GetComponents<IColliderListener>();
            auto colListenerB = pair.b->GetComponents<IColliderListener>();
            // 调用 *iter 里的 A 与 B 的 OnCollisionExit;
            for (auto& comp : colListenerA) if (comp) comp->OnColliderExit(pair.b); 
            for (auto& comp : colListenerB) if (comp) comp->OnColliderExit(pair.a);
        }
    }
    activeCollisions = move(nextActiceCollisions);
    */

    // 帧末尾统一处理删除的Obj
    CleanUp();

    // 统一处理 activeCollisions 已经销毁的条目
    activeCollisions.erase(
        remove_if(activeCollisions.begin(), activeCollisions.end(),
        [](const CollisionPair& p) { return !p.a || !p.b || p.a->isDestroyed || p.b->isDestroyed; }),
        activeCollisions.end()
    );
    // std::cerr << "[Update] After merge & CleanUp: toAdd.size=" << toAdd.size()
    //           << " objects.size=" << objects.size() << std::endl;
}

void Scene::ProcessEvent(const SDL_Event &e)
{
    // Debug::Log("Scene 分发事件");
    for (auto& obj : objects)
    {
        // Debug::Log("分发事件: " + obj->name);
        obj->ProcessEvent(e);
    }
}

/// @brief 按 RendererComponent 里的 Layer 与 OrderInLayer  绘制
/// @param renderer [WARNING] 当前 Render 只会调用 RenderComponent
void Scene::Render(Renderer &renderer)
{ 
    renderer.Clear();

    std::vector<shared_ptr<RendererComponent>> comps;
    comps.reserve(256); // 提前扩充容器以降低开销
    
    // 构建扁平对象，包含所有活跃子物体
    vector<shared_ptr<GameObject>> flatObjects;
    // 预留空间以减少开销
    flatObjects.reserve(objects.size() * 2);
    for (const auto& root : objects)
    {
        CollectObjectChildren(root, flatObjects);
    }
    
    for (const auto& obj : flatObjects)
    {
        if (!obj || obj->isDestroyed) continue;
        auto rcs = obj->GetComponents<RendererComponent>();
        for (const auto& rc : rcs) if (rc) comps.push_back(rc);
    }

    std::stable_sort(comps.begin(), comps.end(),
        [](const shared_ptr<RendererComponent>& a,
                const shared_ptr<RendererComponent>& b)
        {
            if ((int)a->GetLayer() != (int)b->GetLayer()) return (int)a->GetLayer() < (int)b->GetLayer(); 
            return a->GetOrderInLayer() < b->GetOrderInLayer();
        }
    );

    for (const auto& rc : comps) 
    {
        rc->Render(renderer); 
    } 
    renderer.Refresh(); 
}

void Scene::Destroy()
{
    for (auto& obj : objects) { obj->Destroy(); }
    CleanUp();
}

void Scene::DestroyObject(const shared_ptr<GameObject> &obj)
{
    obj->Destroy();
}

void Scene::CleanUp()
{
    objects.erase(
        remove_if(
            objects.begin(), 
            objects.end(), 
            [](const shared_ptr<GameObject>& obj) 
            { 
                return obj->isDestroyed; 
            }
        ), 
        objects.end()
    );
    for (auto& obj : objects) { if (obj) obj->CleanUp(); }
}

shared_ptr<GameObject> Scene::FindFirstByNameInRoot(const string &name)
{
    for (auto& obj : objects)
    {
        if (obj->name == name) return obj;
    }
    Debug::Log("[WARN Scene] FindFirstByNameInRoot not find " + name);
    return nullptr;
}

shared_ptr<GameObject> Scene::InstantiatePrefab(const string &key, const Vector2 &pos)
{
    auto obj = PrefabManager::Get().Instantiate(key);
    if (!obj) 
    {
        Debug::Log("[WARN] Prefab: " + key + " 不存在");
        return nullptr;
    }
    obj->transform.position = pos;
    AddObject(obj);
    return obj;
}

shared_ptr<GameObject> Scene::InstantiatePrefab(const string &key, const Vector2 &pos, const std::function<void(shared_ptr<GameObject>)> &init)
{
    auto obj = PrefabManager::Get().Instantiate(key);
    if (!obj) 
    {
        Debug::Log("[WARN] Prefab: " + key + " 不存在");
        return nullptr;
    }
    obj->transform.SetPosition(pos);
    if (init) 
    {
        Debug::Log("初始化: " + obj->name);
        init(obj);
    }
    AddObject(obj);
    return obj;
}

void Scene::CollectObjectChildren(const shared_ptr<GameObject> &obj, vector<shared_ptr<GameObject>> &out)
{
    if (!obj) return;
    if (obj->isDestroyed) return;
    out.push_back(obj);
    for (auto& c : obj->children)
    {
        CollectObjectChildren(c, out);
    }
}

void Scene::EndGame()
{
    for (auto& obj : objects)
    {
        if (obj->GetTag() != Tag::UI) obj->Destroy();
    }
}
