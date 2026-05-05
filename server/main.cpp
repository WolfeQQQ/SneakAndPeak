#include "GameServer.h"

int main() {
    GameServer server;

    //waiting for GameLoop method
    // std::thread logicThread(GameLoop, &server);
    // logicThread.detach();
    
    server.Start();
    
    return 0;
}