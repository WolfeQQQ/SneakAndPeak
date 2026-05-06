#include "gameLogic.h"
#include "../shared/player.h"
#include "../shared/contstants.h"
#include <iostream>
#include <cmath>

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
        playerMove(players[i], players);
    }
}

//Player move function
void gameLogic::playerMove(player& currentPlayer, player players[4]) {
    //variables
    player::ClientInput currentPlayerInput = currentPlayer.getClientInput();
    float currentSpeed = currentPlayer.getSpeed(), directionX = 0.0f, directionY = 0.0f, adjustedSpeed = currentPlayer.getSpeed();

    //adjusting speed for diagonal movement
    if(currentPlayerInput.up == true) directionY += 1.0f;
    if(currentPlayerInput.down == true) directionY += -1.0f;
    if(currentPlayerInput.right == true) directionX += 1.0f;
    if(currentPlayerInput.left == true) directionX += -1.0f;
    if(directionX == 0 && directionY == 0) return;
    currentSpeed = std::sqrt((directionX * directionX) + (directionY * directionY));
    adjustedSpeed = 1.0f/currentSpeed * currentPlayer.getSpeed();

    //UP
    if(currentPlayerInput.up == true) {
        float direction = currentPlayer.getY() - adjustedSpeed;
        currentPlayer.setY(direction);
        collision(currentPlayer, players, 0);
    }
    //DOWN
    if(currentPlayerInput.down == true) {
        float direction = currentPlayer.getY() + adjustedSpeed;
        currentPlayer.setY(direction);
        collision(currentPlayer, players, 1);
    }
    //RIGHT
    if(currentPlayerInput.right == true) {
        float direction = currentPlayer.getX() + adjustedSpeed;
        currentPlayer.setX(direction);
        collision(currentPlayer, players, 2);
    }
    //LEFT
    if(currentPlayerInput.left == true) {
        float direction = currentPlayer.getX() - adjustedSpeed;
        currentPlayer.setX(direction);
        collision(currentPlayer, players, 3);
    }
}

//collision function
void gameLogic::collision(player& currentPlayer, player players[4], int directionFlag) {
    for(int i = 0; i < 4; i++) {
        if(&currentPlayer == &players[i]) continue;

        //variables for calculating edge points
        float widthPoint = PLAYER_WIDTH/2;
        float lengthPoint = PLAYER_LENGTH/2;
        float currentX = currentPlayer.getX(), currentY = currentPlayer.getY();
        float otherX = players[i].getX(), otherY = players[i].getY(), diffrence = 0.0f;

        //AABB algorithm calculations
        if ((currentX - widthPoint) > (otherX + widthPoint)) continue;
        if ((currentX + widthPoint) < (otherX - widthPoint)) continue;
        if ((currentY - lengthPoint) > (otherY + lengthPoint)) continue;
        if ((currentY + lengthPoint) < (otherY - lengthPoint)) continue;

        //Adjusting current player position based on AABB algorithm calculations
        switch (directionFlag)
        {
        case 0: //up
            diffrence = (otherY + lengthPoint) - (currentY - lengthPoint) + 2.0f;
            currentPlayer.setY(currentPlayer.getY() + diffrence);
            break;
        
        case 1: //down
            diffrence = (currentY + lengthPoint) - (otherY - lengthPoint) + 2.0f;
            currentPlayer.setY(currentPlayer.getY() - diffrence);
            break;

        case 2: //right
            diffrence = (currentX + widthPoint) - (otherX - widthPoint) + 2.0f;
            currentPlayer.setX(currentPlayer.getX() - diffrence);
            break;
            
        case 3: //left
            diffrence = (otherX + widthPoint) - (currentX - widthPoint) + 2.0f;
            currentPlayer.setX(currentPlayer.getX() + diffrence);
            break;

        default:
            break;
        }

        //collision handling
        if(currentPlayer.getIsSeeker() == true) {
            players[i].setIsCaught(true);
            return;
        }
        if(players[i].getIsSeeker() == true) {
            currentPlayer.setIsCaught(true);
            return;
        }
    }
}




 






