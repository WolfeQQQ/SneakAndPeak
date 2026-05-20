#include "GameServer.h"
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

}

//Destructor
GameServer::~GameServer() {Stop(); }

//The main method that creates a socket and handles loops for accepting new clients
void GameServer::Start(){
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
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
    while((readSize = recv(sock, &tempinput, sizeof(player::ClientInput), MSG_WAITALL)) > 0){
         std::lock_guard<std::mutex> lock(stateMutex);
         clientInputs[playerId] = tempinput;
    }

    {
        std::lock_guard<std::mutex> lock(stateMutex);

        ResetPlayer(playerId);

        clientSockets[playerId] = 0;
        std::cout << "Player" << playerId << "left the game. \n";
    }
    close(sock);
}

void GameServer::ResetPlayer(int playerId) {
    players[playerId].setIsConnected(false);
    players[playerId].setIsSeeker(false);
    players[playerId].setIsCaught(false);
    players[playerId].setIsRunning(false);
    players[playerId].setX(500.0f);
    players[playerId].setY(400.0f);
    players[playerId].setStamina(100.0f);
    players[playerId].setDirection(player::Direction::DOWN);
    std::memset(&clientInputs[playerId], 0, sizeof(player::ClientInput));
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

            bool toStop = logic.gameTick(players, this);
            if(toStop) Stop();
        }

        StateToUpload();
        auto endTime = std::chrono::steady_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::this_thread::sleep_for(std::chrono::milliseconds(TICK_DELAY_MS) - frameDuration);
    }
}

//server->StateToUpload()
void GameServer::StateToUpload(){
    std::lock_guard<std::mutex> lock(stateMutex);
    
    GameStatePacket packet;
    packet.stage = currentState;
    packet.timer = stateTimer;
    std::memcpy(packet.players, players, sizeof(players));

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(players[i].getIsConnected() == true){
            // Wysyłamy strukturę pakietu zamiast samej tablicy players
            send(clientSockets[i], &packet, sizeof(GameStatePacket), 0);
        }
    }
}