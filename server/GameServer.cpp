#include "GameServer.h"
#include "gameLogic.h"
#include <iostream>
#include <cstring>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

//Constructor
GameServer::GameServer(){
    isRunning = false;
    listenSocket = -1;
    currentState = GameState::LOBBY;
    stateTimer = 0.0f;
    std::memset(clientSockets, 0, sizeof(clientSockets));
    std::memset(clientInputs, 0, sizeof(clientInputs));
    logic = new gameLogic();

    auto now = std::chrono::steady_clock::now();
    for(int i = 0; i < MAX_CLIENTS; i++){
        lastInputTime[i] = now;
    }
}

//Destructor
GameServer::~GameServer() {Stop(); delete logic;}

//The main method that creates a socket and handles loops for accepting new clients
void GameServer::Start(){
    config.loadConfig("serverConfig.txt");
    
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(config.getPort());
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(listenSocket,10);
    isRunning = true;

    std::thread(&GameServer::GameUpdateLoop, this).detach();

    std::cout << "[Serwer] ready. Waiting for calls";

    while(isRunning){
        int clientSock = accept(listenSocket, nullptr, nullptr);
        if(clientSock >= 0 ){
            HandleNewConnection(clientSock);
        }
    }
}

//Safely close server by releasing port 5000
void GameServer::Stop() {
    isRunning = false;
    if (listenSocket != -1) close(listenSocket);
}

//Handling and assigning parameters of the client that is trying to connect
void GameServer::HandleNewConnection(int clientSock){
    std::lock_guard<std::mutex> lock(stateMutex);
    int assignedId = -1;
    for(int i = 0 ; i < MAX_CLIENTS; i++){
        if(players[i].getIsConnected()!=true){
            assignedId = i;
            clientSockets[i] = clientSock;


            players[i].setIsConnected(true);
            players[i].setId(i);
            players[i].setX(500.0f); 
            players[i].setY(400.0f);
            players[i].setSpeed(4.0f);
            players[i].setStamina(100.0f);
            players[i].setIsCaught(false);
            players[i].setIsSeeker(false);
            players[i].setIsRunning(false);
            

            send(clientSock, &assignedId, sizeof(int), 0);

            break;
        }
        
    }
    if(assignedId == -1){
            close(clientSock);
            return;
    }
    std::cout << "Player" << assignedId <<"admitted\n";
    std::thread(&GameServer::ClientListener, this, assignedId, clientSock).detach();

}


//Listens to data provided by client
void GameServer::ClientListener(int playerId, int sock){
    player::ClientInput tempinput;
    int readSize;

    player::ClientInput localInputCache;
    std::memset(&localInputCache, 0, sizeof(player::ClientInput));
    bool hasNewDataToCommit = false;

    while((readSize = recv(sock, &tempinput, sizeof(player::ClientInput), MSG_WAITALL)) > 0){
        auto now = std::chrono::steady_clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastInputTime[playerId]).count();

        localInputCache = tempinput;
        hasNewDataToCommit = true;

        if(elapsedMs > 10){
            lastInputTime[playerId] = now;
            std::lock_guard<std::mutex> lock(stateMutex);
            clientInputs[playerId] = tempinput;
            hasNewDataToCommit = false;
        }
    }

    {
        std::lock_guard<std::mutex> lock(stateMutex);

        resetPlayer(playerId);

        clientSockets[playerId] = 0;
        std::cout << "Player" << playerId << "left the game. \n";
    }
    close(sock);
}

void GameServer::resetPlayer(int playerId) {
    players[playerId].setIsConnected(false);
    players[playerId].setIsSeeker(false);
    players[playerId].setIsCaught(false);
    players[playerId].setIsRunning(false);
    players[playerId].setX(500.0f);
    players[playerId].setY(400.0f);
    players[playerId].setStamina(100.0f);
    players[playerId].setDirection(player::Direction::DOWN);
    std::memset(&clientInputs[playerId], 0, sizeof(player::ClientInput));
    lastInputTime[playerId] = std::chrono::steady_clock::now();
}

void GameServer::GameUpdateLoop() {
    while(isRunning) {
        auto startTime = std::chrono::steady_clock::now();
        {
            std::lock_guard<std::mutex> lock(stateMutex);
            
            for(int i = 0; i < MAX_CLIENTS; i++) {
                if(players[i].getIsConnected()) {
                    players[i].setInput(clientInputs[i]);
                }
            }

            if(currentState == GameState::GAME_OVER){
                int connectedPlayers = 0;
                bool voteBackToLobby = false;

                for(int i = 0 ; i < MAX_CLIENTS; i++){
                    if(players[i].getIsConnected()){
                        connectedPlayers ++;
                        if(clientInputs[i].backToLobby){
                            voteBackToLobby = true;
                        }
                    }

                }

                if(connectedPlayers == 0){
                    std::cout << "closing server all players left\n";
                    Stop();
                    break;
                }

                if(voteBackToLobby){
                    std::cout << " game reset\n";
                    currentState = GameState::LOBBY;

                    for (int i = 0; i < MAX_CLIENTS; i++) {
                        if (players[i].getIsConnected()) {
                            resetPlayer(i);
                            players[i].setIsConnected(true);
                        }
                    }
                }
            }
            

            if(isRunning){
            bool toStop = logic->gameTick(players, this);
            if(toStop) Stop();
            }
        }

        if (isRunning) {
            StateToUpload();
        }
        auto endTime = std::chrono::steady_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::this_thread::sleep_for(std::chrono::milliseconds(config.getTickDelay()) - frameDuration);
    }
}

//server->StateToUpload()
void GameServer::StateToUpload(){
    std::lock_guard<std::mutex> lock(stateMutex);
    
    GameStatePacket packet;
    packet.stage = currentState;
    packet.timer = logic->getTimeForPlayers();
    std::cout << logic->getTimeForPlayers() << std::endl;
    std::memcpy(packet.players, players, sizeof(players));

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(players[i].getIsConnected() == true){
            // Wysyłamy strukturę pakietu zamiast samej tablicy players
            send(clientSockets[i], &packet, sizeof(GameStatePacket), 0);
        }
    }
}
