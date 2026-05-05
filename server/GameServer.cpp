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
    ClientInput tempinput;
    int readSize;
    while((readSize = recv(sock, &tempinput, sizeof(ClientInput), 0)) > 0){
         std::lock_guard<std::mutex> lock(stateMutex);
         clientInputs[playerId] = tempinput;
    }

    {
        std::lock_guard<std::mutex> lock(stateMutex);
        players[playerId].setIsConnected(false);
        clientSockets[playerId] = 0;
        std::memset(&clientInputs[playerId], 0, sizeof(ClientInput));
        std::cout << "Player" << playerId << "left the game. \n";
    }
    close(sock);
}

//To dla kubara
//server->StateToUpload()
void GameServer::StateToUpload(){
    std::lock_guard<std::mutex> lock(stateMutex);
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(players[i].getIsConnected() == true){
            send(clientSockets[i], players, sizeof(player) * MAX_CLIENTS, 0);
        }
    }
}