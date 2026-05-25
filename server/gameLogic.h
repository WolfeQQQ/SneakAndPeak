#pragma once
#include "../shared/player.h"
#include "../shared/contstants.h"
#include <chrono>
#include <eigen3/Eigen/Dense>
#include <random>

class GameServer;

/**
 * @class gameLogic
 * @brief Manages the core gameplay loop, physics, collisions, and state machine of the game.
 */
class gameLogic
{
public:
    /**
     * @brief Construct a new game Logic object. Initializes random seed and default times.
     */
    gameLogic();

    /**
     * @brief Destroy the game Logic object.
     */
    ~gameLogic();

    //setters

    //getters

    /**
     * @brief Gets the remaining time for the current game stage.
     * * @return float Time left in seconds.
     */
    float getTimeForPlayers() const;
    
    /**
     * @brief Handles main loop of the game. This method should be called per tick.
     * For every player computes movement and collision.
     * Checks and handles players action key.
     * * @param players Array of players to change.
     * @param server Pointer to the parent object that calls this method.
     * @return true if critical error occurred at first initialization.
     * @return false if game tick operates successfully.
     */
    bool gameTick(player players[4], GameServer* server);

private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point lastTime;
    float globalTime;
    float timeForPlayers;
    bool isStarted;
    bool stageStarted;
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> tileMap;
    int seekerIndex;
    std::mt19937 gen;

    /**
     * @brief Calculates and applies movement for a single player based on their input.
     * Normalizes diagonal speed and triggers collision checks after moving.
     * Blocks seeker movement during the COUNTDOWN stage.
     *
     * @param currentPlayer Reference to the player whose movement is being processed.
     * @param players Array of all players (needed for collision checks).
     * @param server Pointer to the GameServer to check current game stage.
     */
    void playerMove(player& currentPlayer, player players[4], GameServer* server);

    /**
     * @brief Checks and resolves collisions between the player, map boundaries, tilemap, and other players.
     * If a Seeker collides with a Hider during the GAME stage, the Hider's state is set to DEATH.
     *
     * @param currentPlayer Reference to the player whose collisions are being checked.
     * @param players Array of all players to check player-to-player collisions.
     * @param directionFlag The current movement direction (used to check specific edge collisions).
     * @param server Pointer to the GameServer to skip player collisions during LOBBY.
     */
    void collision(player& currentPlayer, player players[4], player::Direction directionFlag, GameServer* server);

    /**
     * @brief Performs Axis-Aligned Bounding Box (AABB) collision detection and positional correction.
     * Pushes the player back if a collision is detected to prevent moving through walls/players.
     *
     * @param currentPlayer Reference to the player being checked.
     * @param currentX Current X position of the player.
     * @param currentY Current Y position of the player.
     * @param otherX X position of the colliding object (tile or another player).
     * @param otherY Y position of the colliding object (tile or another player).
     * @param directionFlag Movement direction to determine which side of the bounding box to correct.
     * @param isTileMap Boolean flag. True if colliding with a map tile, false if colliding with another player.
     * @return true if a collision was detected and resolved.
     * @return false if no collision occurred.
     */
    bool aabbAlgorithm(player& currentPlayer, float currentX, float currentY, float otherX, float otherY, player::Direction directionFlag, bool isTileMap);

    /**
     * @brief Handles player stamina regeneration and usage based on movement state.
     * Applies stamina drain when sprinting and regenerates it when walking or resting.
     * Adjusts player speed dynamically based on current stamina level.
     * * @param currentPlayer The player object to be updated.
     * @param deltaTime Time elapsed since the last frame (in seconds).
     */
    void staminaHandler(player& currentPlayer, float deltaTime);

    /**
     * @brief Manages the server's State Machine (Lobby, Countdown, Game, Game Over) and calculates delta time.
     * Handles win/loss conditions, stage transitions, seeker disconnections, and assigns remaining stage time.
     * * @param players Array of all players to check connection status and death states.
     * @param server Pointer to the GameServer to update its global state.
     * @return float Time elapsed since the last tick (deltaTime) in seconds.
     */
    float updateStateAndGetDelta(player players[4], GameServer* server);

    /**
     * @brief Loads tilemap from csv file and modifies tileMap variable.
     * * @return true if critical error occured (ex. file cannot be loaded).
     * @return false if map was loaded succesfully.
     */
    bool loadMap();

    bool isWakable(int tileId);
};

