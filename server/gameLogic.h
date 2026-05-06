#pragma once
#include "../shared/player.h"
#include <chrono>

class gameLogic
{
public:
    gameLogic();
    ~gameLogic();

    //setters

    //getters
    float getGlobalTime() const;
    //methods
    void gameTick(player players[4]);


private:
    //variables
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point lastTime;
    float globalTime;
    bool isStarted;

    //methods
    void playerMove(player& currentPlayer, player players[4]);
    void collision(player& currentPlayer, player players[4], int directionFlag); 
    //collision -> directionFlag: 0 - up, 1 - down, 2 - right, 3 - left
    void staminaHandler(player& currentPlayer, float deltaTime);
};