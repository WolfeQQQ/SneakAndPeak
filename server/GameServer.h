#pragma once

#include <mutex>
#include <thread>
#include <vector>

#include "../shared/contstants.h"
#include "../shared/player.h"
#include "../shared/gameState.h"
#include "gameLogic.h"

struct GameStatePacket {
    GameState stage;
    float timer;
    player players[MAX_CLIENTS];
};

class GameServer {
private:
    int listenSocket;
    bool isRunning;
    gameLogic logic;
    
    std::mutex stateMutex;
    
    //list of my player objects
    player players[MAX_CLIENTS];

    //list of keys from clients
    player::ClientInput clientInputs[MAX_CLIENTS];
    

    int clientSockets[MAX_CLIENTS];

    GameState currentState;
    float stateTimer;

    void HandleNewConnection(int clientSock);
    void ClientListener(int playerId, int sock);
    void GameUpdateLoop();

public:
    GameServer();
    ~GameServer();
    
    void Start();
    void Stop();

    //Allows to get a pointer to the players so he can count collisions
    player* getPlayers() { return players; }
    //Allows to get a pointer to the input list
    player::ClientInput* getInputs() { return clientInputs; }
    //Allows to manually trigger the sending of packets to all players
    void StateToUpload();
    //Allows to block the server while it computes something itself (thread safety)
    std::mutex& getMutex() { return stateMutex; }

    GameState getGameStage() const { return currentState; }
    void setGameStage(GameState stage) { currentState = stage; }
    float getStageTimer() const { return stateTimer; }
    void setStageTimer(float time) { stateTimer = time; }
};