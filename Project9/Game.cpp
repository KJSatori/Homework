#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <functional>
#include "Pizza.h"
#include "Noodle.h"
#include "Grass.h"
#include "Rabbit.h"
#include "Wolf.h"
#include "Tiger.h"

bool Game::IsPosValid(const Vector2Int &pos) const
{
    return !IsPosTakenUpByAnimal(pos) && IsPosInBorder(pos);
}

bool Game::IsPosTakenUpByAnimal(const Vector2Int &pos) const
{
    return (animals.find(pos) != animals.end());
}

bool Game::IsPosInBorder(const Vector2Int &pos) const
{
    return pos.row >= 4 && pos.row <= 23 && pos.col >=1 && pos.col <= 78;
}

template<typename Func>
vector<Vector2Int> Game::FliteredMoves(const vector<Vector2Int> originalMoves, Func filter)
{
    vector<Vector2Int> output;
    for (auto& move: originalMoves) if (filter(move)) output.push_back(move);
    return output;
}

vector<Vector2Int> Game::GetValidGrassPos() const
{
    vector<Vector2Int> output;
    for (int c = 1; c <= 78; c++)
        for (int r = 4; r <= 23; r++)
        {
            Vector2Int pos = Vector2Int(r, c);
            if (grasses.find(pos) == grasses.end()) output.push_back(pos);
        }
    return output;
}

vector<Vector2Int> Game::GetValidAnimalPos() const
{
    vector<Vector2Int> output;
    for (int c = 1; c <= 78; c++)
        for (int r = 4; r <= 23; r++)
        {
            Vector2Int pos = Vector2Int(r, c);
            // pos 没有动物，说明这里可以生成动物
            if (animals.find(pos) == animals.end()) output.push_back(pos);
        }
    return output;
}

Game::Game()
{
    gui.Init();

    for (int c = 1; c <= 78; c++)
        for (int r = 4; r <= 23; r++)
            grasses.insert({Vector2Int(r, c), make_shared<Grass>(r, c)});

    count = 0;

    debugString = "Author: KJSatori - SUAT25000247;\tPress 'A' to generate a Rabbit; \nPress 'S' to generate a Wolf;\t\tPress 'D' to generate a Tiger; ";
}

void Game::Update() {
    
    count++;

    //if (count % 6 == 0)
    {
        // 清屏
        gui.ClearWindow(); 

        int c = gui.ReadInput();
        count = 0;
        day++;

        UpdateDebug();
        UpdateItems(c);
        UpdateUI();
        
        // 将缓存刷入屏幕
        gui.RefreshWindow();
    }
}

void Game::UpdateDebug()
{
    ShowMessageAt(1, 0, debugString);
}

void Game::UpdateUI()
{
    string dayLabel = "";
    string tempdayLabel = "";
    tempdayLabel = " Day " + to_string(day) + " ";
    for (int i = 0; i < (80 - tempdayLabel.length())/2-1; i++) dayLabel += '=';\
    dayLabel += tempdayLabel;
    for (int i = dayLabel.length(); i < 80; i++) dayLabel+= '=';
    ShowMessageAt(0, 0, dayLabel.c_str());

    string marginLabel = "";
    marginLabel += 'O';
    for (int i = 0; i < 78; i++) 
    {
        marginLabel += "-";
    }
    marginLabel += 'O';


    ShowMessageAt(3, 0, marginLabel);

    for (int i = 4; i < 24; i++)
    {
        ShowMessageAt(i, 0, "|");
        ShowMessageAt(i, 79, "|");
    }

    ShowMessageAt(24, 0, marginLabel);
    ShowMessageAt(25, 0, "My wolves and tigers are particularly resistant to hunger.");
    ShowMessageAt(26, 0, "This helps prevent the game from being reviewed by animal rights organizations.");
    ShowMessageAt(27, 0, "Press 'q' to quit.");
}

void Game::UpdateItems(int c) {
    // 接受用户输入
    Vector2Int pos;
    switch (c)
    {
    case 'g':
        pos = GenerateRandomPos(GetValidGrassPos());
        if (pos != Vector2Int(0, 0)) grasses.insert({pos, make_shared<Grass>(pos.row, pos.col)});
        break;
    case 'a':
        pos = GenerateRandomPos(GetValidAnimalPos());
        if (pos != Vector2Int(0, 0)) animals.insert({pos, make_shared<Rabbit>(pos.row, pos.col)});
        break;
    case 's':
        pos = GenerateRandomPos(GetValidAnimalPos());
        if (pos != Vector2Int(0, 0)) animals.insert({pos, make_shared<Wolf>(pos.row, pos.col)});
        break;
    case 'd':
        pos = GenerateRandomPos(GetValidAnimalPos());
        if (pos != Vector2Int(0, 0)) animals.insert({pos, make_shared<Tiger>(pos.row, pos.col)});
        break;
    case 'q':
        isGameEnd = true;
    }

    for (auto& [key, value]: grasses)
    {
        value->Update();
        PlaceItem(*value);
    }

    // 所有移动的Animal
    unordered_map<Vector2Int, shared_ptr<Animal>, Vector2IntHash> movedAnimal;
    for (auto iter = animals.begin(); iter != animals.end();)
    {
        auto& animal = iter->second;
        Vector2Int oldPos = iter->first;

        // 检查 已经移动的动物，但是还没有刷新到 unorderedmap 的动物有没有位置重复 
        auto isTaken = [&](const Vector2Int& pos)
        {
            return movedAnimal.find(pos) != movedAnimal.end();
        };

        vector<Vector2Int> possiblePoses = animal->PossibleMoves(animals);
        vector<Vector2Int> fliteredMoves;
        if (animal->GetType() == AnimalType::Rabbit) fliteredMoves = FliteredMoves(possiblePoses, [&](const Vector2Int& pos) { return IsPosValid(pos) && !isTaken(pos); });
        else fliteredMoves =  FliteredMoves(possiblePoses, [&](const Vector2Int& pos) { return IsPosInBorder(pos); });

        // animal默认留在原地
        Vector2Int targetMove = animal->GetPos();
        // 标记是否需要 erase iter
        bool willEraseCurrentIter = false;
        
        if (!fliteredMoves.empty())
        {
            targetMove = fliteredMoves[rand()%fliteredMoves.size()];

            // 如果目标位置在 animals，说明有猎物或其他动物
            auto preyIt = animals.find(targetMove);
            if (preyIt != animals.end())
            {
                // 防止自己吃自己
                if (preyIt == iter) {; }
                else
                {
                    // 吃掉猎物
                    animal->Eat();
                    preyIt->second->Destroy();

                    // 立即从 animals 中移除被吃掉的猎物
                    animals.erase(preyIt);

                    // 如果movedAnimal出现了被吃猎物，则也需要删除
                    auto movedIt = movedAnimal.find(targetMove);
                    if (movedIt != movedAnimal.end()) movedAnimal.erase(movedIt);
                }
            }
            else
            {
                // 如果目标不在 animals 也可能在 movedAnimal
                auto movedIt = movedAnimal.find(targetMove);
                if (movedIt != movedAnimal.end())
                {
                    // 吃掉 movedAnimal 中的动物
                    animal->Eat();
                    movedIt->second->Destroy();
                    movedAnimal.erase(movedIt);
                }
            }
        }

        animal->Update(targetMove);
        Vector2Int newPos = animal->GetPos();

        if (newPos != oldPos)
        {
            movedAnimal.insert_or_assign(newPos, animal);
            iter = animals.erase(iter);
        }
        else ++iter;
    }

    // 最后把 movedAnimal 合并回 animals，使用覆盖语义
    for (auto& [p, a] : movedAnimal) {
        animals.insert_or_assign(p, a);
    }

    for (auto iter = animals.begin(); iter != animals.end(); )
    {
        auto& animal = iter->second;
        switch (animal->GetType())
        {
        case AnimalType::Rabbit:
            if (grasses.find(animal->GetPos()) != grasses.end())
            {
                shared_ptr<Grass> grass = grasses[animal->GetPos()];
                if (grass->GetState() == grass->GetMaxState())
                {
                    grass->ResetState();
                    animal->Eat();
                }
            }
            break;
        case AnimalType::Wolf:
            break;
        case AnimalType::Tiger:
            break;
        }
        if (animal->GetLifecycle() == 0) 
        {
            iter = animals.erase(iter);
        }
        else ++iter;
    }

    /* 草没必要删除，重新生长
    for (auto iter = grasses.begin(); iter != grasses.end();)
    {
        auto& grass = iter->second;

        if (grass->GetLifecycle() == 0) 
        {
            iter = grasses.erase(iter);
        }
        else ++iter;
    }
    */

    for (auto& [pos, animal]: animals)
    {
        PlaceItem(*animal, true);
    }
}

void Game::PlaceItem(const Item& item, bool isBold)
{
    Vector2Int pos = item.GetPos();

    PlaceTokenAt(pos.row, pos.col, item.GetSymbol(), item.GetColorPair(), isBold);
}

Vector2Int Game::GenerateRandomPos(vector<Vector2Int> validPoses)
{
    if (validPoses.size() != 0)
        return validPoses[rand()%validPoses.size()];
    else return Vector2Int(0, 0);
}

/// @brief 在对应地方放置单个字符
void Game::PlaceTokenAt(size_t row, size_t col, char token, int colorPair, bool isBold) {
    if (!isBold) gui.DrawCharAt(row, col, token, colorPair);
    else gui.DrawBoldCharAt(row, col, token, colorPair);
}

/// @brief 在对应地方显示一串消息
void Game::ShowMessageAt(size_t row, size_t col, string message) {
    gui.DrawTextAt(row, col, message.c_str());
}