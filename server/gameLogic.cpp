#include "gameLogic.h"
#include "../shared/player.h"

gameLogic::gameLogic()
{
}

gameLogic::~gameLogic()
{
}

//Main server logic  
void gameLogic::gameTick(player players[4]) {
    
    for(int i = 0; i < 4; i++) {
        if(players[i].getIsConnected() == false) continue;
        int oldX = players[i].getX();
        int oldY = players[i].getY();
        playerMove(players[i]);
        collision(players[i], players, oldX, oldY);
    }



}

//Player move function
void gameLogic::playerMove(player& currentPlayer) {
    player::ClientInput currentPlayerInput = currentPlayer.getClientInput();
    //UP
    if(currentPlayerInput.up == true) currentPlayer.setY(-currentPlayer.getSpeed());
    //DOWN
    else if(currentPlayerInput.down == true) currentPlayer.setY(currentPlayer.getSpeed());
    //RIGHT
    else if(currentPlayerInput.right == true) currentPlayer.setX(currentPlayer.getSpeed());
    //LEFT
    else if(currentPlayerInput.left == true) currentPlayer.setX(-currentPlayer.getSpeed());
    else return;
}

//collision function
void gameLogic::collision(player& currentPlayer, player players[4], int oldX, int oldY) {
    for(int i = 0; i < 3; i++) {
        if(&currentPlayer == &players[i]) continue;
        

    }
}




 






