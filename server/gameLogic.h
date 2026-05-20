#pragma once
#include "../shared/player.h"
#include "../shared/contstants.h"
#include "../shared/gameState.h"
#include <chrono>
#include <eigen3/Eigen/Dense>
#include <random>

class GameServer;

class gameLogic
{
public:
    gameLogic();
    ~gameLogic();

    //setters

    //getters
    float getGlobalTime() const;
    //methods
    bool gameTick(player players[4], GameServer* server);


private:
    //variables
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point lastTime;
    float globalTime;
    bool isStarted;
    bool stageStarted;
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> tileMap;
    int seekerIndex;
    std::mt19937 gen;

    //methods
    void playerMove(player& currentPlayer, player players[4], GameServer* server);

    /*
        all methods for collision checking (TileMap and other players)
    
        //collision -> directionFlag: 0 - up, 1 - down, 2 - right, 3 - left
    */
    void collision(player& currentPlayer, player players[4], int directionFlag, GameServer* server);
    bool aabbAlgorithm(player& currentPlayer, float currentX, float currentY, float otherX, float otherY, int directionFlag);
    void aabbAlgorithmTileMap(player& currentPlayer, float currentX, float currentY, float otherX, float otherY, int directionFlag); 
    void checkCollision(player& currentPlayer, float currentX, float currentY, int playerPosOnGridX, int playerPosOnGridY, float directionFlag);

    //Stamina hanlder
    void staminaHandler(player& currentPlayer, float deltaTime);

    //Game State manager
    float stateManager(player players[4], GameServer* server);

    //map loader
    bool loadMap();

    //sudden disconnections
    bool suddenDisconnections(player players[4], GameServer* server, GameState stage);
};