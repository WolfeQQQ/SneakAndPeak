#pragma once
#include "screen.h"
#include "network.h"
#include "raylib.h"
#include "../shared/gameState.h"

class GameOverScreen : public Screen {
private:
    Network* network;
    GameStatePacket lastPacket; 
    bool backToLobbyPressed;
    int optionsCount = 2;
    int selectedOption = 0;

public:
    GameOverScreen(Network* networkClient, const GameStatePacket& finalPacket);
    
    AppState update() override;
    void draw() override;
    ~GameOverScreen();
};