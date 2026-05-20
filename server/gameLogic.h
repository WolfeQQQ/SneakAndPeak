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
    
    /**
     * @brief Handles main loop of the game. This method should be called per tick
     * For every player compute movement and collision
     * Checks and handles players action key
     * @return true if critical error occurred at first initialization
     * @return false if game tick operates successfully
     *
     * @param players Array of players to change
     * @param server Pointer to the parent object that calls this method
     */
    bool gameTick(player players[4], GameServer* server);


private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point lastTime;
    float globalTime;
    bool isStarted;
    bool stageStarted;
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> tileMap;
    int seekerIndex;
    std::mt19937 gen;

    /**
     * @brief
     *
     * @param 
     * @param 
     * @param 
     */
    void playerMove(player& currentPlayer, player players[4], GameServer* server);

    /**
     * @brief
     *
     * @param 
     * @param 
     * @param 
     * @param 
     */
    void collision(player& currentPlayer, player players[4], player::Direction directionFlag, GameServer* server);

    /**
     * @brief
     *
     * @param 
     * @param 
     * @param 
     * @param 
     * @param 
     * @param 
     */
    bool aabbAlgorithm(player& currentPlayer, float currentX, float currentY, float otherX, float otherY, player::Direction directionFlag, bool isTileMap);

    /**
     * @brief
     *
     * @param 
     * @param 
     * @param 
     * @param 
     * @param 
     * @param 
     */
    void aabbAlgorithmTileMap(player& currentPlayer, float currentX, float currentY, float otherX, float otherY, player::Direction directionFlag);

    /**
     * @brief Handles player stamina regeneration and usage based on movement state.
     * Applies stamina drain when sprinting and regenerates it when walking or resting.
     * Adjusts player speed dynamically based on current stamina level.
     * 
     * @param currentPlayer The player object to be updated.
     * @param deltaTime Time elapsed since the last frame (in seconds).
     */
    void staminaHandler(player& currentPlayer, float deltaTime);

    /**
     * @brief
     * 
     * @param
     * @param
     */
    float updateStateAndGetDelta(player players[4], GameServer* server);

    /**
     * @brief Loads tilemap from csv file and modifies tileMap variable
     * @return true if critical error occured (ex. file cannot be loaded)
     * @return false if map was loaded succesfully
     */
    bool loadMap();

    
};