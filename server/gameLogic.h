#pragma once
#include "../shared/player.h"

class gameLogic
{
public:
    gameLogic();
    ~gameLogic();
    void gameTick(player players[4]);

private:
    void playerMove(player& currentPlayer);
    void collision(player& currentPlayer, player players[4], int oldX, int oldY);
};