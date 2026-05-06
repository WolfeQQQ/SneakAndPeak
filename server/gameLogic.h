#pragma once
#include "../shared/player.h"

class gameLogic
{
public:
    gameLogic();
    ~gameLogic();
    void gameTick(player players[4]);

private:
    void playerMove(player& currentPlayer, player players[4]);
    void collision(player& currentPlayer, player players[4], int directionFlag); 
    //collision -> directionFlag: 0 - up, 1 - down, 2 - right, 3 - left
};