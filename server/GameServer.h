#pragma once

#include <mutex>
#include <thread>
#include <vector>

#include "../shared/contstants.h"
#include "../shared/player.h"
#include "../shared/gameState.h"


// Forward declaration to break potential circular dependencies
class gameLogic;

/**
 * @class GameServer
 * @brief Core network server responsible for managing client connections, input streaming, and server tick synchronization.
 */
class GameServer {
private:
    int listenSocket;
    bool isRunning;
    gameLogic* logic;
    
    std::mutex stateMutex;
    
    //list of my player objects
    player players[MAX_CLIENTS];

    //list of keys from clients
    player::ClientInput clientInputs[MAX_CLIENTS];
    

    int clientSockets[MAX_CLIENTS];

    std::chrono::steady_clock::time_point lastInputTime[MAX_CLIENTS];

    GameState currentState;
    float stateTimer;

    /**
     * @brief Allocates a network slot, sets initial player state, and spawns a dedicated listening thread for the client.
     * @param clientSock The file descriptor of the freshly accepted client socket.
     */
    void HandleNewConnection(int clientSock);

    /**
     * @brief Dedicated worker thread task running per client to continuously receive incoming network streams.
     * @param playerId The uniquely assigned server slot identifier for this client.
     * @param sock The network socket file descriptor bound to this client connection.
     */
    void ClientListener(int playerId, int sock);
    
    /**
     * @brief Fixed-timestep continuous update loop processing physics updates and tick dispatches.
     */
    void GameUpdateLoop();
    
public:
    /**
     * @brief Constructor allocating internal sub-modules and resetting connection vectors.
     */
    GameServer();
    
    /**
     * @brief Destructor terminating threads and freeing allocated logic subsystems.
     */
    ~GameServer();
    
    
    /**
     * @brief Binds the listening socket and enters the primary block accepting incoming connections.
     */
    void Start();
    
    /**
     * @brief Safely shuts down listening threads and releases allocated system sockets.
     */
    void Stop();

    /**
     * @brief Exposes a pointer to the current player entity state data.
     * @return Raw pointer to the internal array of player objects.
     */
    player* getPlayers() { return players; }
    
    
    /**
     * @brief Exposes a pointer to the buffered client inputs.
     * @return Raw pointer to the continuous input data array.
     */
    player::ClientInput* getInputs() { return clientInputs; }
    
    /**
     * @brief Serializes the authoritative game state and broadasts packets to all connected network clients.
     */
    void StateToUpload();
    
    /**
     * @brief Grants external access to the primary internal synchronization primitive.
     * @return Reference to the main state protection std::mutex.
     */
    std::mutex& getMutex() { return stateMutex; }

    // Getters and Setters
    GameState getGameStage() const { return currentState; }
    void setGameStage(GameState stage) { currentState = stage; }
    float getStageTimer() const { return stateTimer; }
    void setStageTimer(float time) { stateTimer = time; }
    void setIsRunning(bool running){  isRunning = running; }
    /**
     * @brief Reinitializes player positions, flags, and cleans the associated memory allocations upon disconnect or reset.
     * @param playerId Assigned system indexing identifier of the player.
     */
    void resetPlayer(int playerId);


};