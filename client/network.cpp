#include "network.h"
#include <iostream>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

// Constructor, initializes the network object
Network::Network(){
    sock = -1;
    connected = false;
}

// Destructor, closes the socket if open
Network::~Network() {
    Disconnect();
}

// Connects the client to the specified server and retrieves the player ID
bool Network::ConnectToServer(const std::string& ip, int port, int *playerId){
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{}; 
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if(inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
        return false;
    }

    if(connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        return false;
    }

    // After connecting, the server should send the client its assigned player ID
    int myId = -1;
    if (recv(sock, &myId, sizeof(int), 0) <= 0) {
        std::cerr << "[Network] Error while fetching player ID from server!\n";
        close(sock);
        sock = -1;
        return false;
    }

    // SUCCESS! the client is now connected to the server and has received its player ID
    *playerId = myId;
    connected = true;
    return true;
}

// Closes the connection to the server and cleans up resources
void Network::Disconnect() {
    if(connected && sock != -1) {
        close(sock);
        sock = -1;
        connected = false;
    }
}

// Sends the player's input to the server
void Network::SendInput(player::ClientInput input) {
    if (!connected || sock == -1) return ;

    // Send the ClientInput structure to the server
    // Returns an error if the send operation fails
    if (send(sock, &input, sizeof(player::ClientInput), 0) < 0) {
        std::cerr << "[Network] Error while sending input to server!\n";
    }
}

// Receives the current game state from the server
bool Network::ReceiveState(GameStatePacket& packet) {
    if(!connected || sock == -1) return false;
    
    // Use recv with MSG_PEEK to check if a full GameStatePacket is available before actually receiving it
    // Returns an error if the connection is closed or if there's an issue with receiving data
    int available = recv(sock, &packet, sizeof(GameStatePacket), MSG_PEEK |MSG_DONTWAIT);
    if (available == 0) {
        std::cerr << "[Network] Connection closed by server.\n";
        Disconnect();
        return false;
    } 

    // If a full GameStatePacket is available, receive it and return true. Otherwise, return false to indicate that no new game state is available yet.
    if (available == sizeof(GameStatePacket)) {
        recv(sock, &packet, sizeof(GameStatePacket), MSG_DONTWAIT);
        return true;
    }
    return false;
}


