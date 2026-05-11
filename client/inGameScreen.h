#pragma once
#include "screen.h"
#include "../shared/player.h"
#include "network.h"
#include "raylib.h"

class InGameScreen : public Screen {
    private:
        Camera2D camera;
        player myPlayer;
        player players[4]; 
        Network* network;
        int playerId;
    public:
        InGameScreen(Network* networkClient, int playerId);
        AppState update() override;
        void draw() override;
    
};