#include "gameLogic.h"
#include "GameServer.h"
#include "../shared/player.h"
#include "../shared/contstants.h"
#include <iostream>
#include <cmath>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <sstream> 
#include <string>

gameLogic::gameLogic()
{
    isStarted = false;
}

gameLogic::~gameLogic()
{
}

//Getters
float gameLogic::getGlobalTime() const {
    return this->globalTime;
}

//Main server logic  
bool gameLogic::gameTick(player players[4]) {

    //Game time start with first inicialization
    auto currentTime = std::chrono::steady_clock::now();
    if(isStarted == false) {
        bool crashFlag = loadMap();
        if(crashFlag) return true;
        startTime = currentTime;
        lastTime = startTime;
        isStarted = true;
        return false;
    }

    //Game time calculations
    std::chrono::duration<float> elapsed = currentTime - startTime;
    globalTime = elapsed.count();
    std::chrono::duration<float> frameDelta = currentTime - lastTime;
    lastTime = currentTime;
    float deltaTime = frameDelta.count();

    //for-loop computing outcome for every player
    for(int i = 0; i < 4; i++) {
        if(players[i].getIsConnected() == false || players[i].getIsCaught() == true) continue;
        staminaHandler(players[i], deltaTime);
        playerMove(players[i], players);
    }
    return false;
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

//collision function with players
void gameLogic::collision(player& currentPlayer, player players[4], int directionFlag) {

    //variables to calculate collsion
    float currentX = currentPlayer.getX(), currentY = currentPlayer.getY();

    //min max X and min max Y
    int playerPosOnGridXMin = (int)currentPlayer.getX()/ TILE_SIZE;
    int playerPosOnGridYMin = (int)currentPlayer.getY()/ TILE_SIZE;
    int playerPosOnGridXMax = ((int)currentPlayer.getX() + PLAYER_WIDTH) / TILE_SIZE;
    int playerPosOnGridYMax = ((int)currentPlayer.getY() + PLAYER_LENGTH) / TILE_SIZE;
    
    switch (directionFlag) {
    case 0: //UP

        //tile map collision check
        if(tileMap(playerPosOnGridYMin, playerPosOnGridXMin) != -1) {
            checkCollision(currentPlayer, currentX, currentY, playerPosOnGridXMin, playerPosOnGridYMin, directionFlag);
            break;
        }
        if(tileMap(playerPosOnGridYMin, playerPosOnGridXMax) != -1) {
            checkCollision(currentPlayer, currentX, currentY, playerPosOnGridXMax, playerPosOnGridYMin, directionFlag);
            break;
        };

        break;
    
    case 1: //DOWN
        //tile map collision check
        if(tileMap(playerPosOnGridYMax, playerPosOnGridXMin) != -1) {
            checkCollision(currentPlayer, currentX, currentY, playerPosOnGridXMin, playerPosOnGridYMax, directionFlag);
            break;
        }
        if(tileMap(playerPosOnGridYMax, playerPosOnGridXMax) != -1) {
            checkCollision(currentPlayer, currentX, currentY, playerPosOnGridXMax, playerPosOnGridYMax, directionFlag);
            break;
        };
        break;
        
    case 2: // RIGHT

        //tile map collision check
        if(tileMap(playerPosOnGridYMin, playerPosOnGridXMax) != -1) {
            checkCollision(currentPlayer, currentX, currentY, playerPosOnGridXMax, playerPosOnGridYMin, directionFlag);
            break;
        }
        if(tileMap(playerPosOnGridYMax, playerPosOnGridXMax) != -1) {
            checkCollision(currentPlayer, currentX, currentY, playerPosOnGridXMax, playerPosOnGridYMax, directionFlag);
            break;
        };
        break;
    
    case 3: //LEFT

        //tile map collision check
        if(tileMap(playerPosOnGridYMin, playerPosOnGridXMin) != -1) {
            checkCollision(currentPlayer, currentX, currentY, playerPosOnGridXMin, playerPosOnGridYMin, directionFlag);
            break;
        }
        if(tileMap(playerPosOnGridYMax, playerPosOnGridXMin) != -1) {
            checkCollision(currentPlayer, currentX, currentY, playerPosOnGridXMin, playerPosOnGridYMax, directionFlag);
            break;
        };

    default:
        break;
    }

    for(int i = 0; i < 4; i++) {
        if(&currentPlayer == &players[i]) continue;

        //variables for calculating edge points
        float otherX = players[i].getX(), otherY = players[i].getY();

        aabbAlgorithm(currentPlayer, currentX, currentY, otherX, otherY, directionFlag);

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

//AABB algorithm function for players
void gameLogic::aabbAlgorithm(player& currentPlayer, float currentX, float currentY, float otherX, float otherY, int directionFlag) {

    float widthPoint = PLAYER_WIDTH/2;
    float lengthPoint = PLAYER_LENGTH/2;
    float diffrence = 0.0f;

    //AABB algorithm calculations
    if ((currentX - widthPoint) > (otherX + widthPoint)) return;
    if ((currentX + widthPoint) < (otherX - widthPoint)) return;
    if ((currentY - lengthPoint) > (otherY + lengthPoint)) return;
    if ((currentY + lengthPoint) < (otherY - lengthPoint)) return;

    //Adjusting current player position based on AABB algorithm calculations
    switch (directionFlag)
    {
    case 0: //up
        diffrence = (otherY + lengthPoint) - (currentY - lengthPoint) + 0.1f;
        currentPlayer.setY(currentPlayer.getY() + diffrence);
        break;
        
    case 1: //down
        diffrence = (currentY + lengthPoint) - (otherY - lengthPoint) + 0.1f;
        currentPlayer.setY(currentPlayer.getY() - diffrence);
        break;

    case 2: //right
        diffrence = (currentX + widthPoint) - (otherX - widthPoint) + 0.1f;
        currentPlayer.setX(currentPlayer.getX() - diffrence);
        break;
            
    case 3: //left
        diffrence = (otherX + widthPoint) - (currentX - widthPoint) + 0.1f;
        currentPlayer.setX(currentPlayer.getX() + diffrence);
        break;

    default:
        break;
    }
}

//AABB algorithm function for tilemap
void gameLogic::aabbAlgorithmTileMap(player& currentPlayer, float currentX, float currentY, float otherX, float otherY, int directionFlag) {
    float diffrence = 0.0f;

    //AABB algorithm calculations
    // if ((currentX) > (otherX + (float)(TILE_SIZE))) return;
    if ((currentX + PLAYER_WIDTH) < (otherX)) return;
    if ((currentY) > (otherY + (float)(TILE_SIZE))) return;
    if ((currentY + PLAYER_LENGTH) < (otherY)) return;

    // Adjusting current player position based on AABB algorithm calculations
    switch (directionFlag)
    {
    case 0: //up
        diffrence = (otherY + (float)(TILE_SIZE)) - (currentY) + 0.1f;
        currentPlayer.setY(currentPlayer.getY() + diffrence);
        break;
        
    case 1: //down
        diffrence = (currentY + PLAYER_LENGTH) - (otherY) + 0.1f;
        currentPlayer.setY(currentPlayer.getY() - diffrence);
        break;

    case 2: //right
        diffrence = (currentX + PLAYER_WIDTH) - (otherX) + 0.1f;
        currentPlayer.setX(currentPlayer.getX() - diffrence);
        break;
            
    case 3: //left
        diffrence = (otherX + (float)(TILE_SIZE)) - (currentX) + 0.1f;
        currentPlayer.setX(currentPlayer.getX() + diffrence);
        break;

    default:
        break;
    }
}

//players stamina handler
void gameLogic::staminaHandler(player& currentPlayer, float deltaTime) {

    //if(currentPlayer.getIsSeeker() == false) return;

    //variables
    player::ClientInput currentPlayerInput = currentPlayer.getClientInput();
    float stamina = currentPlayer.getStamina();
    float currentSpeed = currentPlayer.getSpeed();

    /*
        *@brief: stamina usage and regeneration handler
    
        takin into account 5 states -> 
        * key is clicked and stamina > 0
        * key is clicked and stamina < 0
        * key is not clicked and player is running (player no longer holds shift_key)
        * key is not clicked and stamina < MAX_STAMINA
        * key is not clicked and stamina > MAX_STAMINA
    */
    if(currentPlayerInput.shift == true) {
        //First state
        if(stamina > 0.0f) {
            float currentStamina = stamina - (DRAIN_RATE * deltaTime);
            currentPlayer.setStamina(currentStamina);
            if(currentPlayer.getIsRunning() == false) {
                currentPlayer.setSpeed(currentSpeed * 1.5);
                currentPlayer.setIsRunning(true);
            }
            return;
        }
        //Second state
        else {
            if(currentPlayer.getIsRunning() == true) {
                currentPlayer.setSpeed(currentSpeed / 1.5);
                if(stamina < 0.0f) currentPlayer.setStamina(0.0f);
                currentPlayer.setIsRunning(false);
            }
            return;
        }
    }
    else {
        //Third state
        if(currentPlayerInput.shift == false && currentPlayer.getIsRunning() == true) {
            currentPlayer.setSpeed(currentSpeed / 1.5);
            currentPlayer.setIsRunning(false);
        }
        //Fourth state
        if(stamina < MAX_STAMINA) {
            float currentStamina = stamina + (REGEN_RATE * deltaTime);
            currentPlayer.setStamina(currentStamina);
            return;
        }
        //Fifth state
        else {
            currentPlayer.setStamina(MAX_STAMINA);
            return;
        }
    }
}

//loading tilemap
bool gameLogic::loadMap() {

    std::ifstream plik("assets/map_temp.csv");

    //error message no data to load
    if(!plik.is_open()) {
        std::cout << "CRITICAL ERROR! YO MAMA2FAT TO OPEN" << std::endl;
        return true;
    }

    tileMap.resize(MAP_HEIGHT, MAP_WIDTH);

    std::string linia;
    int y = 0;

    //loading rows
    while (std::getline(plik, linia) && y < MAP_HEIGHT) {
        
        //variables
        std::stringstream ss(linia);
        std::string komorka;
        int x = 0;

        //loading rows
        while (std::getline(ss, komorka, ',') && x < MAP_WIDTH) {
            
            //adding cuted row (for example "1")
            tileMap(y, x) = std::stoi(komorka);
            x++;
        }
        y++;
    }
    std::cout << "\nMapa wczytana pomyslnie!" << std::endl;
    return false;
}

//Tile map collsion method
void gameLogic::checkCollision(player& currentPlayer, float currentX, float currentY, int playerPosOnGridX, int playerPosOnGridY, float directionFlag) {
    //variables for calculating edge points
        float otherX = (float)playerPosOnGridX * TILE_SIZE, otherY = (float)playerPosOnGridY * TILE_SIZE;

        aabbAlgorithmTileMap(currentPlayer, currentX, currentY, otherX, otherY, directionFlag);
}
 






