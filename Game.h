#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


#include <stdio.h>
#include "Board.h"
#include <vector>


class Game
{
private:
    Board board;


public:
    Game();
    void playConsole();

    char getCurrentSymbol(int);

    void run();
};

#endif // GAME_H_INCLUDED
