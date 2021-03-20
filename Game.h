#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include "Image.h"



class Game
{
private:
    int level_number;
    Image level;

    Image DrawLevel(char* filename);
public:
    StartGame();
    NextLevel();
    FailGame();
    FinishGame();

    Game(/* args */);
    ~Game();
};

Game::Game(/* args */)
{
}

Game::~Game()
{
}



#endif //MAIN_GAME_H