#pragma once
#include "screen.h"
#include "../shared/player.h"
#include "network.h"
#include "raylib.h"
#include "tilemap.h"

class InGameScreen : public Screen {
    private:
        Camera2D camera;
        player myPlayer;
        player players[4]; 
        Network* network;
        int playerId;
        Tilemap tilemap;
    public:
        InGameScreen(Network* networkClient, int playerId);
        AppState update() override;
        void draw() override;
    
};