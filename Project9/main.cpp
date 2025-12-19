#include <ncurses.h>
#include "Gui.h"
#include <sys/time.h>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <cstddef>
#include "Game.h"
using namespace std;

int ParseArgOrDefault(char* argv[], int index, int def)
{
    if (!argv[index]) return def;
    try {
        int v = stoi(argv[index]);
        return v < 0 ? def : v;
    } catch (...) {
        return def;
    }
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    struct timeval time_now;

    int initTigers = 0;
    int initWolves = 0;
    int initRabbits = 0;

    if (argc > 1) initTigers = ParseArgOrDefault(argv, 1, 0);
    if (argc > 2) initWolves = ParseArgOrDefault(argv, 2, 0);
    if (argc > 3) initRabbits = ParseArgOrDefault(argv, 3, 0);

    gettimeofday(&time_now, NULL);
    time_t oldTime = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
    time_t currentTime =  oldTime;
    Game game;
    game.InitAnimals(initTigers, initWolves, initRabbits);
    while (true) {
        gettimeofday(&time_now, NULL);
        currentTime = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);

        if (currentTime - oldTime < 67) 
        {
            usleep(1000);
            continue;
        }

        game.Update();
        if (game.IsGameEnd()) break;

        oldTime = currentTime;
    }
    return 0;
}
