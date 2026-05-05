#pragma once
#include "objectClasses/player.h"

class gameLogic
{
private:
    /* data */
public:
    gameLogic();
    ~gameLogic();
    void gameTick(player players[4]);
    void playerMove(player currentPlayer);
};