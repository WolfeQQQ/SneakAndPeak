#include "gameLogic.h"
#include "GameServer.h"
#include "../shared/player.h"
#include "../shared/contstants.h"
#include "../shared/gameState.h"
#include <iostream>
#include <cmath>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <sstream> 
#include <string>
#include <algorithm>
#include <random>

gameLogic::gameLogic() {
    isStarted = false;
    stageStarted = false;
    timeForPlayers = LOBBY_TIME;

    //random seed initialization
    std::random_device rd; 
    std::mt19937 gen(rd());
}

gameLogic::~gameLogic() {}

float gameLogic::getTimeForPlayers() const {
    return this->timeForPlayers;
}

void gameLogic::staminaHandler(player& currentPlayer, float deltaTime) {

    player::ClientInput currentPlayerInput = currentPlayer.getClientInput();
    float stamina = currentPlayer.getStamina();
    float currentSpeed = currentPlayer.getSpeed();

    //Sprint handling
    if(currentPlayerInput.shift) {
        if(stamina > 0.0f) {
            float currentStamina = stamina - (DRAIN_RATE * deltaTime);
            currentPlayer.setStamina(currentStamina);
            if(currentPlayer.getIsSeeker() && !currentPlayer.getIsRunning()) {
                currentPlayer.setSpeed(currentSpeed * 1.5);
                currentPlayer.setIsRunning(true);
            }
            else if(!currentPlayer.getIsSeeker() && !currentPlayer.getIsViewing()) {
                currentPlayer.setViewing(true);
            }
            return;
        }
        //Stamina depleted while trying to sprint
        else {
            if(currentPlayer.getIsSeeker() && currentPlayer.getIsRunning()) {
                currentPlayer.setSpeed(currentSpeed / 1.5);
                if(stamina < 0.0f) currentPlayer.setStamina(0.0f); // Clamp to prevent negative stamina
                currentPlayer.setIsRunning(false);
            }
            else if(!currentPlayer.getIsSeeker() && currentPlayer.getIsViewing()) {
                currentPlayer.setViewing(false);
            }
            return;
        }
    }
    // Handle Walking
    else {
        if(!currentPlayerInput.shift && currentPlayer.getIsRunning()) {
            currentPlayer.setSpeed(currentSpeed / 1.5);
            currentPlayer.setIsRunning(false);
            currentPlayer.setViewing(false);
        }
        if(!currentPlayerInput.shift && currentPlayer.getIsViewing()) {
            currentPlayer.setViewing(false);
        }
        if(stamina < MAX_STAMINA) {
            float currentStamina = stamina + (REGEN_RATE * deltaTime);
            currentPlayer.setStamina(currentStamina);
            return;
        }
        else {
            currentPlayer.setStamina(MAX_STAMINA); // Clamp to max capacity
            return;
        }
    }
}

bool gameLogic::loadMap() {

    std::ifstream plik("assets/map_temp.csv");

    if(!plik.is_open()) {
        std::cout << "CRITICAL ERROR! YO MAMA2FAT TO OPEN" << std::endl;
        return true;
    }

    //pre-allocation for security and efficency
    tileMap.resize(MAP_HEIGHT, MAP_WIDTH);

    std::string linia;
    int y = 0;

    //parsing line to line to ensure file size
    while (std::getline(plik, linia) && y < MAP_HEIGHT) {
        
        std::stringstream ss(linia);
        std::string komorka;
        int x = 0;

        while (std::getline(ss, komorka, ',') && x < MAP_WIDTH) {
            
            tileMap(y, x) = std::stoi(komorka);
            x++;
        }
        y++;
    }
    std::cout << "\nMapa wczytana pomyslnie!" << std::endl;
    return false;
}

bool gameLogic::gameTick(player players[4], GameServer* server) {

    if(!isStarted) {
        bool crashFlag = loadMap();
        if(crashFlag) return true;
        isStarted = true;
        return false; //First time do not compute anything for safety reasons
    }

    float deltaTime = updateStateAndGetDelta(players, server);

    for(int i = 0; i < 4; i++) {
        if(!players[i].getIsConnected() || players[i].getPlayerState() == player::PlayerState::DEATH) continue;
        staminaHandler(players[i], deltaTime);
        playerMove(players[i], players, server);
    }
    return false;
}

void gameLogic::playerMove(player& currentPlayer, player players[4], GameServer* server) {

    player::ClientInput currentPlayerInput = currentPlayer.getClientInput();
    float currentSpeed = currentPlayer.getSpeed();
    float directionX = 0.0f;
    float directionY = 0.0f;

    if(currentPlayer.getIsSeeker() && server->getGameStage() == GameState::COUNTDOWN) return;

    if(currentPlayerInput.down) directionY += 1.0f;
    if(currentPlayerInput.up) directionY += -1.0f;
    if(currentPlayerInput.right) directionX += 1.0f;
    if(currentPlayerInput.left) directionX += -1.0f;

    if(directionX == 0 && directionY == 0) {
        currentPlayer.setPlayerState(player::PlayerState::IDLE);
        return;
    }

    float magnitude = std::sqrt((directionX * directionX) + (directionY * directionY));
    float adjustedSpeed = 1.0f/magnitude * currentSpeed;

    currentPlayer.setPlayerState(player::PlayerState::WALK);

    if(directionY != 0) {
        player::Direction animationDir = directionY > 0.0f ? player::Direction::DOWN : player::Direction::UP;
        float newY = currentPlayer.getY() + (adjustedSpeed * directionY);
        
        currentPlayer.setY(newY);
        currentPlayer.setDirection(animationDir);

        collision(currentPlayer, players, animationDir, server);
    }

    if(directionX != 0) {
        player::Direction animationDir = directionX > 0.0f ? player::Direction::RIGHT : player::Direction::LEFT;
        float newX = currentPlayer.getX() + (adjustedSpeed * directionX);
        
        currentPlayer.setX(newX);
        currentPlayer.setDirection(animationDir);

        collision(currentPlayer, players, animationDir, server);
    }
}

void gameLogic::collision(player& currentPlayer, player players[4], player::Direction directionFlag, GameServer* server) {

    float currentX = currentPlayer.getX(), currentY = currentPlayer.getY();

    int playerPosOnGridXMin = (int)currentPlayer.getX()/ TILE_SIZE;
    int playerPosOnGridYMin = (int)currentPlayer.getY()/ TILE_SIZE;
    int playerPosOnGridXMax = ((int)currentPlayer.getX() + PLAYER_WIDTH) / TILE_SIZE;
    int playerPosOnGridYMax = ((int)currentPlayer.getY() + PLAYER_LENGTH) / TILE_SIZE;

    if(currentX < 0 || currentY < 0 || currentX + PLAYER_WIDTH >= MAP_WIDTH * TILE_SIZE || currentY + PLAYER_LENGTH >= MAP_HEIGHT * TILE_SIZE) {
        currentPlayer.setPlayerState(player::PlayerState::DEATH);
        return;
    }

    if(directionFlag == player::Direction::UP || directionFlag == player::Direction::DOWN) {

        int otherY = directionFlag == player::Direction::UP ? playerPosOnGridYMin : playerPosOnGridYMax;
        float otherX = -1.0f;

        if(tileMap(otherY, playerPosOnGridXMin) != -1) otherX = (float)playerPosOnGridXMin * TILE_SIZE;
        else if(tileMap(otherY, playerPosOnGridXMax) != -1) otherX = (float)playerPosOnGridXMax * TILE_SIZE;

        if(otherX != -1.0f) aabbAlgorithm(currentPlayer, currentX, currentY, otherX, (float)(otherY * TILE_SIZE), directionFlag, true);
    }

    if(directionFlag == player::Direction::RIGHT || directionFlag == player::Direction::LEFT) {

        int otherX = directionFlag == player::Direction::LEFT ? playerPosOnGridXMin : playerPosOnGridXMax;
        float otherY = -1.0f;

        if(tileMap(playerPosOnGridYMin, otherX) != -1) otherY = (float)playerPosOnGridYMin * TILE_SIZE;
        else if(tileMap(playerPosOnGridYMax, otherX) != -1) otherY = (float)playerPosOnGridYMax * TILE_SIZE;

        if(otherY != -1.0f) aabbAlgorithm(currentPlayer, currentX, currentY, (float)(otherX * TILE_SIZE), otherY, directionFlag, true);
    }

    if(server->getGameStage() == GameState::LOBBY) return;
    for(int i = 0; i < 4; i++) {
        if(&currentPlayer == &players[i]) continue;
        if(players[i].getPlayerState() == player::PlayerState::DEATH) continue;

        float otherX = players[i].getX(), otherY = players[i].getY();

        bool isColliding = aabbAlgorithm(currentPlayer, currentX, currentY, otherX, otherY, directionFlag, false);
        if (!isColliding) continue;
        
        if(currentPlayer.getIsSeeker() == true) {
            players[i].setPlayerState(player::PlayerState::DEATH);
            return;
        }
        if(players[i].getIsSeeker() == true) {
            currentPlayer.setPlayerState(player::PlayerState::DEATH);
            return;
        }
    }
}

bool gameLogic::aabbAlgorithm(player& currentPlayer, float currentX, float currentY, float otherX, float otherY, player::Direction directionFlag, bool isTileMap) {

    float diffrence = 0.0f;
    float width;
    float length;

    if(isTileMap) {
        width = (float)TILE_SIZE;
        length = (float)TILE_SIZE;
    }
    else {
        width = (float)PLAYER_WIDTH;
        length = (float)PLAYER_LENGTH;
    }

    //AABB algorithm calculations
    if ((currentX) > (otherX + width)) return false;
    if ((currentX + PLAYER_WIDTH) < (otherX)) return false;
    if ((currentY) > (otherY + length)) return false;
    if ((currentY + PLAYER_LENGTH) < (otherY)) return false;

    //Adjusting current player position based on AABB algorithm calculations
    switch (directionFlag)
    {
    case player::Direction::UP:
        diffrence = (otherY + length) - (currentY) + 0.1f;
        currentPlayer.setY(currentPlayer.getY() + diffrence);
        break;
        
    case player::Direction::DOWN:
        diffrence = (currentY + PLAYER_LENGTH) - (otherY) + 0.1f;
        currentPlayer.setY(currentPlayer.getY() - diffrence);
        break;

    case player::Direction::RIGHT:
        diffrence = (currentX + PLAYER_WIDTH) - (otherX) + 0.1f;
        currentPlayer.setX(currentPlayer.getX() - diffrence);
        break;
            
    case player::Direction::LEFT:
        diffrence = (otherX + width) - (currentX) + 0.1f;
        currentPlayer.setX(currentPlayer.getX() + diffrence);
        break;

    default:
        break;
    }
    return true;
}

//game state manager method TODO (optimalization)
float gameLogic::updateStateAndGetDelta(player players[4], GameServer* server) {

    auto currentTime = std::chrono::steady_clock::now();
    int connectedPlayers = std::count_if(players, players + 4, [](player& p) {
        return p.getIsConnected();
    });

    if(!stageStarted) {
        startTime = currentTime;
        lastTime = startTime;
        stageStarted = true;

        //drawing seeker
        if(server->getGameStage() == GameState::COUNTDOWN) {
            int possibleSeekers[4];
            int count = 0;
            for(int i = 0; i < 4; i++) {
                if(players[i].getIsConnected()) {
                    possibleSeekers[count] = i;
                    count++;
                }
            }
            std::uniform_int_distribution<> distrib(0, count - 1);
            seekerIndex = possibleSeekers[distrib(gen)];
            players[seekerIndex].setIsSeeker(true);
        }
    }

    std::chrono::duration<float> elapsed = currentTime - startTime;
    globalTime = elapsed.count();

    //game state handler
    switch (server->getGameStage()) {
        case GameState::LOBBY: {
            timeForPlayers = LOBBY_TIME - globalTime;
            //first check
            if(connectedPlayers < 4 && globalTime <= LOBBY_TIME) break;

            if(connectedPlayers == 0) {
                startTime = currentTime;
                lastTime = startTime;
                break;
            }

            if(connectedPlayers < 2) {
                server->setIsRunning(false);
                break;
            }

            stageStarted = false;
            server->setGameStage(GameState::COUNTDOWN);

            break;
        }
        case GameState::COUNTDOWN: {

            timeForPlayers = COUNTDOWN_TIME - globalTime;
            bool seekerOnline = players[seekerIndex].getIsConnected();

            if(connectedPlayers < 2 || !seekerOnline){

                if(seekerOnline) std::cout << "Not enough players to continue countdown.\n";
                else std::cout << "Seeker left before start. Players back to the lobby \n";

                server->setGameStage(GameState::LOBBY);
                for (int i = 0; i < 4; i++) {
                    if (players[i].getIsConnected()) {
                        server->resetPlayer(i);
                        players[i].setIsConnected(true);
                    }
                }
                stageStarted = false;
                break;
            }

            if(globalTime > COUNTDOWN_TIME) {
                stageStarted = false;
                server->setGameStage(GameState::GAME);
            }
            break;
        }
        case GameState::GAME: {
            //caught players
            int caughtPlayers = std::count_if(players, players + 4, [](player& p) {
                return p.getPlayerState() == player::PlayerState::DEATH;
            });
            timeForPlayers = GAME_TIME - globalTime;

            bool seekerOnline = players[seekerIndex].getIsConnected();

            if(!seekerOnline){
                std::cout << "Seeker left the game. Hiders win. \n";

                server->setGameStage(GameState::GAME_OVER);
                stageStarted = false;
                break;
            }
            
            //next stage (if game lasts more than 3 minutes or every hider is caught)
            if(globalTime > GAME_TIME || caughtPlayers + 1 == connectedPlayers) {
                stageStarted = false;
                server->setGameStage(GameState::GAME_OVER);
            }
            break;
        }
        case GameState::GAME_OVER:

            timeForPlayers = GAME_OVER_TIME - globalTime;

            if(globalTime > GAME_OVER_TIME) {
                server->setIsRunning(false);
                break;
            }
            break;    
        default:
            break;
    }

    std::chrono::duration<float> frameDelta = currentTime - lastTime;
    lastTime = currentTime;
    return frameDelta.count();
}

void gameLogic::runRadar(player players[4], float deltaTime){
    radarTimer += deltaTime;

    if(radarTimer >= 5.0f){
        radarTimer = 0.0f;

        int seekerId = -1;

        for(int i = 0; i < 4; i++){
            if(players[i].getIsConnected() && players[i].getIsSeeker()){
                seekerId = i;
                break;
            }
        }

        if(seekerId > -1){
            float seekerX = players[seekerId].getX();
            float seekerY = players[seekerId].getY();

            int choosenHider = -1;
            float curr_dist = 999999.0f;

            for(int i = 0; i < 4 ; i++){
                if(i = seekerId || !players[i].getIsConnected() || players[i].getPlayerState() == player::PlayerState::DEATH){
                    continue;
                }

                float hidderX = players[i].getX();
                float hidderY = players[i].getY();

                float difX = seekerX - hidderX;
                float difY = seekerY - hidderY;

                float dist = std::sqrt(difX * difX + difY * difY);

                if(dist < curr_dist){
                    curr_dist = dist;
                    choosenHider = i;
                }
            }

            if(choosenHider != -1){
                std::cout << "X: " << players[choosenHider].getX();
                std::cout << "Y: " << players[choosenHider].getY();
            }
        }
    }
}