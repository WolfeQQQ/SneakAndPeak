#include "game_logic.h"
#include "objectClasses/player.h"
#include "../shared/protocol.h"

gameLogic::gameLogic()
{
}

gameLogic::~gameLogic()
{
}

void gameTick(player players[4]) {
    
    for(int i = 0; i < 4; i++) {
        if(players[i].getIsConnected() == false) continue;
        int oldX = players[i].getX();
        int oldY = players[i].getY();
        playerMove(players[i]);
    }



}

void playerMove(player currentPlayer) {
    
}







