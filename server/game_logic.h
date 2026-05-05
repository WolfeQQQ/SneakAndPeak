#pragma once
#include "../shared/player.h"

class gameLogic
{
private:
    /* data */
public:
    gameLogic();
    ~gameLogic();
    void gameTick(player);
    void playerMove(player);
};