#pragma once

#include <mutex>
#include <thread>
#include <vector>

#include "../shared/contstants.h"
#include "../shared/player.h"

class GameServer {
private:
    int listenSocket;
    bool isRunning;
    
    std::mutex stateMutex;
    
    //list of my player objects
    player players[MAX_CLIENTS];

    //list of keys from clients
    ClientInput clientInputs[MAX_CLIENTS];
    

    int clientSockets[MAX_CLIENTS];

    void HandleNewConnection(int clientSock);
    void ClientListener(int playerId, int sock);

public:
    GameServer();
    ~GameServer();
    
    void Start();
    void Stop();

    //Allows to get a pointer to the players so he can count collisions
    player* getPlayers() { return players; }
    //Allows to get a pointer to the input list
    ClientInput* getInputs() { return clientInputs; }
    //Allows to manually trigger the sending of packets to all players
    void StateToUpload();
    //Allows to block the server while it computes something itself (thread safety)
    std::mutex& getMutex() { return stateMutex; }
};