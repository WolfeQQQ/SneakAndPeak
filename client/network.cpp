
#include "network.h"
#include <iostream>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

Network::Network(){
    sock = -1;
    connected = false;
}

Network::~Network() {
    Disconnect();
}

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

    int myId = -1;
    
    if (recv(sock, &myId, sizeof(int), 0) <= 0) {
        std::cerr << "[Network] Blad pobierania ID gracza od serwera!\n";
        close(sock);
        sock = -1;
        return false;
    }

    *playerId = myId;
    std::cout << "[Network] Polaczono! Moje ID to: " << myId << "\n";
    
    connected = true;

    return true;
}

void Network::Disconnect() {
    if(connected && sock != -1) {
        close(sock);
        sock = -1;
        connected = false;
    }
}

void Network::SendInput(player::ClientInput input) {
    if (!connected || sock == -1) return;

    if (send(sock, &input, sizeof(player::ClientInput), 0) < 0) {
        std::cerr << "[Network] Blad podczas wysylania klawiszy!\n";
    }
}

void Network::ReceiveState(player players[4]) {
    if(!connected || sock == -1) return;
    
    int bytesRead = recv(sock, players, sizeof(player) * 4, MSG_DONTWAIT);

    if (bytesRead > 0) {
        std::cout << "[Network] Otrzymano stan od serwera.\n";
    } else if (bytesRead == 0) {
        std::cerr << "[Network] Polaczenie zamkniete przez serwer.\n";
        Disconnect();
    } 

}


