#pragma once
#include "../shared/player.h"
#include "../shared/contstants.h"
#include <chrono>
#include <eigen3/Eigen/Dense>

class gameLogic
{
public:
    gameLogic();
    ~gameLogic();

    //setters

    //getters
    float getGlobalTime() const;
    //methods
    bool gameTick(player players[4]);


private:
    //variables
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point lastTime;
    float globalTime;
    bool isStarted;
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> tileMap;

    //methods
    void playerMove(player& currentPlayer, player players[4]);
    void collision(player& currentPlayer, player players[4], int directionFlag);
    void aabbAlgorithm(player& currentPlayer, float currentX, float currentY, float otherX, float otherY, int directionFlag);
    void aabbAlgorithmTileMap(player& currentPlayer, float currentX, float currentY, float otherX, float otherY, int directionFlag); 
    //collision -> directionFlag: 0 - up, 1 - down, 2 - right, 3 - left
    void checkCollision(player& currentPlayer, float currentX, float currentY, int playerPosOnGridX, int playerPosOnGridY, float directionFlag);
    void staminaHandler(player& currentPlayer, float deltaTime);
    bool loadMap();
};