#include "gameLogic.h"
#include "../shared/player.h"
#include "../shared/contstants.h"

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
    player::ClientInput currentPlayerInput = currentPlayer.getClientInput();
    for(int i = 0; i < 4; i++) {
        if(&currentPlayer == &players[i]) continue;

        //variables for calculating edge points
        float widthPoint = PLAYER_WIDTH/2;
        float lengthPoint = PLAYER_LENGTH/2;
        float currentX = currentPlayer.getX(), currentY = currentPlayer.getY();
        float otherX = players[i].getX(), otherY = players[i].getY();

        //AABB algorithm calculations
        if ((currentX - widthPoint) > (otherX + widthPoint)) return;
        if ((currentX + widthPoint) < (otherX - widthPoint)) return;
        if ((currentY - lengthPoint) > (otherY + lengthPoint)) return;
        if ((currentY + lengthPoint) < (otherY - lengthPoint)) return;

        //collision handling
        if(currentPlayer.getIsSeeker() == true) {
            players[i].setIsCaught(true);
            return;
        }
        if(players[i].getIsSeeker() == true) {
            currentPlayer.setIsCaught(true);
            return;
        }

        //Adjusting current player position based on AABB algorithm calculations
        if(currentPlayerInput.right == true) {
            int diffrence = (currentX + widthPoint) - (otherX - widthPoint) - 1;
            currentPlayer.setX(-diffrence);
        }
        if(currentPlayerInput.left == true) {
            int diffrence = (otherX + widthPoint) - (currentX - widthPoint) + 1;
            currentPlayer.setX(diffrence);
        }
        if(currentPlayerInput.down == true) {
            int diffrence = (currentY + lengthPoint) - (otherY - lengthPoint) - 1;
            currentPlayer.setX(-diffrence);
        }
        if(currentPlayerInput.up == true) {
            int diffrence = (otherY + lengthPoint) - (currentY - lengthPoint) + 1;
            currentPlayer.setX(diffrence);
        }
    }
}




 






