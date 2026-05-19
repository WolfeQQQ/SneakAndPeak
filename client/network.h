#pragma once 

#include <string>
#include "../shared/player.h"
#include "../shared/gameState.h"

class Network{
    private:
        int sock;
        bool connected;

    public:
        Network();
        ~Network();

        bool ConnectToServer(const std::string& ip, int port, int *playerId);
        void Disconnect();
        bool IsConnected() const { return connected; }
        void SendInput(player::ClientInput input);
        void ReceiveState(GameStatePacket& packet);



};