#ifndef REGISTERPREFAB_H
#define REGISTERPREFAB_H

#include "../../Engine/Core/Prefab/PrefabManager.h"
#include "../../Engine/Core/GameObject/GameObject.h"
#include "../../Engine/Core/Component/RendererComponent.h"
#include "../../Engine/Core/Component/InputHandler.h"
#include "../../Engine/Core/Component/Rigidbody.h"
#include "../../Engine/Core/Component/Collider.h"
#include "../../Engine/Core/Component/PlayerController.h"

#include "../Scripts/Bullet.h"
#include "../Scripts/Player.h"
#include "../Scripts/Enemy.h"
#include "../Scripts/EnemySmallRandomer.h"
#include "../Scripts/EnemyBezierSniper.h"
#include "../Scripts/EnemyBezierFixer.h"
#include "../Scripts/Item.h"
#include "../Scripts/ScoreLabel.h"
#include "../Scripts/PowerLabel.h"
#include "../Scripts/LifeLabel.h"

#include <functional>
#include <vector>

/// @brief 必须要在加载处注册
void RegisterPlayerPrefab()
{
    Prefab player;
    player.key = "player";
    player.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Player", Tag::Player);

        auto rendererComponent = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        rendererComponent->SetSprite("YellowPlayerShip");
        rendererComponent->SetSize(0.0f, 0.0f);
        rendererComponent->SetScaleFactor(3.0f);
        rendererComponent->SetColor(SDL_Color{255, 255, 255, 255});
        rendererComponent->SetApplyTransform(true);
        rendererComponent->SetLayer(RenderLayer::Default);
        obj->AddComponent(rendererComponent);

        auto triggerRenderer = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        triggerRenderer->SetSprite("SlowEffect");
        triggerRenderer->SetScaleFactor(1.25f);
        triggerRenderer->SetColor(SDL_Color{255, 255, 255, 0});
        triggerRenderer->SetApplyTransform(true);
        triggerRenderer->SetLayer(RenderLayer::Foreground);
        obj->AddComponent(triggerRenderer);

        auto rigidbody = std::make_shared<Rigidbody>(obj.get());
        obj->AddComponent(rigidbody);

        auto inputHandler = std::make_shared<InputHandler>(obj.get());
        obj->AddComponent(inputHandler);

        auto playerController = std::make_shared<PlayerController>(obj.get(), rigidbody, inputHandler,337.5f);
        obj->AddComponent(playerController);

        auto collider = std::make_shared<Collider>(obj.get());
        // 绀珠传灵梦碰撞箱大小
        collider->SetCircle(obj->transform.position, 2.5f);
        obj->AddComponent(collider);

        auto playerScript = std::make_shared<Player>(obj.get(), rendererComponent, triggerRenderer);
        obj->AddComponent(playerScript);

        obj->transform.position = playerScript->GetSpawnPoint();

        return obj;
    };

    PrefabManager::Get().Register(player.key, player);
}

void RegisterBulletPrefab()
{
    Prefab bullet;
    bullet.key = "bullet";
    bullet.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Bullet", Tag::PlayerBullet);

        // Renderer
        auto rendererComponent = std::make_shared<RendererComponent>(obj.get(), RenderKind::Rectangle);
        // 绀珠传封魔针大小
        rendererComponent->SetSize(6.0f, 80.0f);
        rendererComponent->SetColor(SDL_Color{255, 220, 80, 255});
        rendererComponent->SetApplyTransform(true);
        obj->AddComponent(rendererComponent);

        // Collider
        auto collider = std::make_shared<Collider>(obj.get());
        collider->SetRectangle(obj->transform.position, 5.0f, 80.0f);
        obj->AddComponent(collider);

        // Bullet
        auto bullet = std::make_shared<Bullet>(obj.get());
        obj->AddComponent(bullet);

        return obj;
    };

    PrefabManager::Get().Register(bullet.key, bullet);
}

/// @brief 子弹模板 Tag 碰撞箱大小 图像索引 都需要设置
void RegisterBulletTemplatePrefab()
{
    Prefab templateBullet;
    templateBullet.key = "templateBullet";
    templateBullet.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Template Bullet" /*, Tag*/);

        // Renderer
        auto rendererComponent = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        // rendererComponent->SetSprite("YellowMiddleJade");
        rendererComponent->SetApplyTransform(true);
        // rendererComponent->SetScaleFactor(1.0f);
        rendererComponent->SetColor(SDL_Color{255, 255, 255, 255});
        rendererComponent->SetLayer(RenderLayer::Bullet);
        obj->AddComponent(rendererComponent);

        auto collider = std::make_shared<Collider>(obj.get());
        // collider->SetCircle(obj->transform.position, 3.5f);
        obj->AddComponent(collider);

        auto bullet = std::make_shared<Bullet>(obj.get());
        // bullet->SetParameters(1200.0f, 1, Vector2::Up());
        // bullet->SetSpawnOffset(Vector2(0, 15.0f));
        obj->AddComponent(bullet);

        return obj;
    };

    PrefabManager::Get().Register(templateBullet.key, templateBullet);
}

void RegisterPlayerYellowBulletPrefab()
{
    Prefab playerYellowBullet;
    playerYellowBullet.key = "playerYellowBullet";
    playerYellowBullet.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Player Yellow Bullet", Tag::PlayerBullet);

        // Renderer
        auto rendererComponent = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        rendererComponent->SetSprite("YellowMiddleJade");
        rendererComponent->SetApplyTransform(true);
        rendererComponent->SetScaleFactor(2.0f);
        rendererComponent->SetColor(SDL_Color{255, 255, 255, 255});
        rendererComponent->SetLayer(RenderLayer::Bullet);
        obj->AddComponent(rendererComponent);

        auto collider = std::make_shared<Collider>(obj.get());
        collider->SetCircle(obj->transform.position, 9.0f);
        obj->AddComponent(collider);

        auto bullet = std::make_shared<Bullet>(obj.get());
        bullet->SetParameters(1200.0f, 1, Vector2::Up());
        bullet->SetSpawnOffset(Vector2(0, 15.0f));
        obj->AddComponent(bullet);

        return obj;
    };

    PrefabManager::Get().Register(playerYellowBullet.key, playerYellowBullet);
}

void RegisterEnemyPrefab()
{
    Prefab enemy;
    enemy.key = "enemy";
    enemy.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Enemy", Tag::Enemy);

        obj->transform.position = Vector2(200.0f, 100.0f);

        auto rendererComponent = std::make_shared<RendererComponent>(obj.get(), RenderKind::Circle);
        rendererComponent->SetRadius(10.0f);
        rendererComponent->SetColor(SDL_Color{80, 220, 255, 255});
        rendererComponent->SetApplyTransform(true);
        obj->AddComponent(rendererComponent);

        auto rigidbody = std::make_shared<Rigidbody>(obj.get());
        obj->AddComponent(rigidbody);

        auto collider = make_shared<Collider>(obj.get());
        collider->SetCircle(obj->transform.position, 9.5f);
        obj->AddComponent(collider);
         
        auto enemyScript = make_shared<Enemy>(obj.get());
        obj->AddComponent(enemyScript);

        return obj;
    };

    PrefabManager::Get().Register(enemy.key, enemy);
}

void RegisterEnemySmallRandomerPrefab()
{
    Prefab enemySmallRandomer;
    enemySmallRandomer.key = "enemySmallRandomer";
    enemySmallRandomer.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Enemy small randomer", Tag::Enemy);

        obj->transform.position = Vector2(200.0f, 100.0f);

        auto rendererComponent = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        rendererComponent->SetSprite("EnemySmallRandomer");
        rendererComponent->SetApplyTransform(true);
        rendererComponent->SetScaleFactor(2.0f);
        obj->AddComponent(rendererComponent);

        auto rigidbody = std::make_shared<Rigidbody>(obj.get());
        obj->AddComponent(rigidbody);

        auto collider = make_shared<Collider>(obj.get());
        collider->SetCircle(obj->transform.position, 7.0f);
        obj->AddComponent(collider);
         
        auto enemyScript = make_shared<EnemySmallRandomer>(obj.get());
        obj->AddComponent(enemyScript);

        return obj;
    };

    PrefabManager::Get().Register(enemySmallRandomer.key, enemySmallRandomer);
}

void RegisterEnemyBezierSniper()
{
    Prefab enemyBezierSniper;
    enemyBezierSniper.key = "enemyBezierSniper";
    enemyBezierSniper.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Enemy bezier sniper", Tag::Enemy);

        auto render = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        render->SetSprite("EnemySmallSniper");
        render->SetApplyTransform(true);
        render->SetScaleFactor(3.0f);
        obj->AddComponent(render);

        auto rigidbody = std::make_shared<Rigidbody>(obj.get());
        obj->AddComponent(rigidbody);

        auto collider = make_shared<Collider>(obj.get());
        collider->SetCircle(obj->transform.position, 10.0f);
        obj->AddComponent(collider);

        auto enemyScript = make_shared<EnemyBezierSniper>(obj.get());
        /* 实例化后额外调用 Init 以 SetParameter */
        obj->AddComponent(enemyScript);

        return obj;
    };

    PrefabManager::Get().Register(enemyBezierSniper.key, enemyBezierSniper);
}

void RegisterEnemyBezierFixer()
{
    Prefab enemyBezierFixer;
    enemyBezierFixer.key = "enemyBezierFixer";
    enemyBezierFixer.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Enemy bezier fixer", Tag::Enemy);
        
        auto render = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        render->SetSprite("EnemySmallFixer");
        render->SetApplyTransform(true);
        render->SetScaleFactor(3.0f);
        obj->AddComponent(render);

        auto rigidbody = std::make_shared<Rigidbody>(obj.get());
        obj->AddComponent(rigidbody);

        auto collider = make_shared<Collider>(obj.get());
        collider->SetCircle(obj->transform.position, 10.0f);
        obj->AddComponent(collider);

        auto enemyScript = make_shared<EnemyBezierFixer>(obj.get());
        /* 实例化后调用 Init 以 SetParameter */
        obj->AddComponent(enemyScript);

        return obj;
    };

    PrefabManager::Get().Register(enemyBezierFixer.key, enemyBezierFixer);
}

void RegisterScoreItem()
{
    Prefab scoreItem;
    scoreItem.key = "scoreItem";
    scoreItem.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Score Item", Tag::Item);
        
        auto render = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        render->SetSprite("Score");
        render->SetApplyTransform(true);
        render->SetScaleFactor(1.25f);
        obj->AddComponent(render);

        auto item = make_shared<Item>(obj.get());
        item->type = ItemType::Score;
        item->value = 10;
        obj->AddComponent(item);

        return obj;
    };
    PrefabManager::Get().Register(scoreItem.key, scoreItem);
}

void RegisterPowerItem()
{
    Prefab powerItem;
    powerItem.key = "powerItem";
    powerItem.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Power Item", Tag::Item);
        
        auto render = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        render->SetSprite("Power");
        render->SetApplyTransform(true);
        render->SetScaleFactor(1.25f);
        obj->AddComponent(render);

        auto item = make_shared<Item>(obj.get());
        item->type = ItemType::Power;
        item->value = 5;
        obj->AddComponent(item);

        return obj;
    };
    PrefabManager::Get().Register(powerItem.key, powerItem);
}

void RegisterLifeItem()
{
    Prefab lifeItem;
    lifeItem.key = "lifeItem";
    lifeItem.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Life Item", Tag::Item);
        
        auto render = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        render->SetSprite("Life");
        render->SetApplyTransform(true);
        render->SetScaleFactor(1.25f);
        obj->AddComponent(render);

        auto item = make_shared<Item>(obj.get());
        item->type = ItemType::Life;
        item->value = 1;
        obj->AddComponent(item);

        return obj;
    };
    PrefabManager::Get().Register(lifeItem.key, lifeItem);
}

void RegisterBombItem()
{
    Prefab bombItem;
    bombItem.key = "bombItem";
    bombItem.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Bomb Item", Tag::Item);
        
        auto render = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        render->SetSprite("Bomb");
        render->SetApplyTransform(true);
        render->SetScaleFactor(1.25f);
        obj->AddComponent(render);

        auto item = make_shared<Item>(obj.get());
        item->type = ItemType::Bomb;
        item->value = 1;
        obj->AddComponent(item);

        return obj;
    };
    PrefabManager::Get().Register(bombItem.key, bombItem);
}

void RegisterScoreLabel()
{
    Prefab scoreLabel;
    scoreLabel.key = "scoreLabel";
    scoreLabel.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Score Label", Tag::UI);

        auto render = std::make_shared<RendererComponent>(obj.get(), RenderKind::Text);
        render->SetFontKey("UIFont");
        render->SetColor({0,0,0,255});
        render->SetCenter(false);
        render->SetLayer(RenderLayer::UI);
        render->SetOrderInLayer(10);
        render->SetText("Score unset");
        obj->AddComponent(render);

        auto scoreLabel = std::make_shared<ScoreLabel>(obj.get(), render);
        obj->AddComponent(scoreLabel);

        return obj;
    };

    PrefabManager::Get().Register(scoreLabel.key, scoreLabel);
}

void RegisterPowerLabel()
{
    Prefab powerLabel;
    powerLabel.key = "powerLabel";
    powerLabel.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Power Label", Tag::UI);

        auto render = std::make_shared<RendererComponent>(obj.get(), RenderKind::Text);
        render->SetFontKey("UIFont");
        render->SetColor({0,0,0,255});
        render->SetCenter(false);
        render->SetLayer(RenderLayer::UI);
        render->SetOrderInLayer(10);
        render->SetText("Power unset");
        obj->AddComponent(render);

        auto powerLabel = std::make_shared<PowerLabel>(obj.get(), render);
        obj->AddComponent(powerLabel);

        return obj;
    };

    PrefabManager::Get().Register(powerLabel.key, powerLabel);
}

void RegisterLifeLabel()
{
    Prefab lifeLabel;
    lifeLabel.key = "lifeLabel";
    lifeLabel.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Life Label", Tag::UI);

        std::vector<std::shared_ptr<RendererComponent>> lifeRenders;
        Vector2 offset = Vector2(20.0f, 0.0f);
        for (int i = 0; i < 8; i++)
        {
            auto render = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
            render->SetSprite("LifeSmall");
            render->SetColor({255,255,255,255});
            render->SetOffset(i * offset);
            render->SetCenter(true);
            render->SetScaleFactor(1.25f);
            render->SetLayer(RenderLayer::UI);
            render->SetOrderInLayer(10);
            obj->AddComponent(render);

            lifeRenders.push_back(render);
        }

        auto lifeLabel = std::make_shared<LifeLabel>(obj.get(), lifeRenders);
        obj->AddComponent(lifeLabel);

        return obj;
    };

    PrefabManager::Get().Register(lifeLabel.key, lifeLabel);
}

#include "../Scripts/BombLabel.h"

void RegisterBombLabel()
{
    Prefab bombLabel;
    bombLabel.key = "bombLabel";
    bombLabel.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = std::make_shared<GameObject>("Bomb Label", Tag::UI);

        std::vector<std::shared_ptr<RendererComponent>> bombRenders;
        Vector2 offset = Vector2(20.0f, 0.0f);
        for (int i = 0; i < 8; i++)
        {
            auto render = std::make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
            render->SetSprite("BombSmall");
            render->SetColor({255,255,255,255});
            render->SetOffset(i * offset);
            render->SetCenter(true);
            render->SetScaleFactor(1.25f);
            render->SetLayer(RenderLayer::UI);
            render->SetOrderInLayer(10);
            obj->AddComponent(render);

            bombRenders.push_back(render);
        }

        auto bombLabel = std::make_shared<BombLabel>(obj.get(), bombRenders);
        obj->AddComponent(bombLabel);

        return obj;
    };

    PrefabManager::Get().Register(bombLabel.key, bombLabel);
}

#include "../Scripts/BackGroundScroller.h"

void RegisterScrollBG()
{
    Prefab scrollBG;
    scrollBG.key = "scrollBG";
    scrollBG.factory = []() -> std::shared_ptr<GameObject>
    {
        auto obj = make_shared<GameObject>("Background", Tag::UI);
        obj->transform.SetPosition(Vector2::Zero());
        auto bgRenderer = make_shared<RendererComponent>(obj.get(), RenderKind::Sprite);
        bgRenderer->SetSprite("Background");
        bgRenderer->SetColor(SDL_Color{150, 150, 150, 255});
        bgRenderer->SetCenter(false);
        bgRenderer->SetLayer(RenderLayer::BackGround);
        obj->AddComponent(bgRenderer);

        auto bgScoller = std::make_shared<BackGroundScroller>(obj.get());
        obj->AddComponent(bgScoller);

        return obj;
    };

    PrefabManager::Get().Register(scrollBG.key, scrollBG);
}

#endif