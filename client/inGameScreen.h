#pragma once
#include "screen.h"
#include "../shared/player.h"
#include "network.h"
#include "raylib.h"
#include "tilemap.h"


  
struct PlayerAnimation {
    player::PlayerState lastState = player::PlayerState::IDLE;
    player::PlayerState state = player::PlayerState::IDLE;
    player::Direction direction = player::Direction::DOWN;

    int frame = 0;
    float frameTimer = 0.0f;

};




class InGameScreen : public Screen {
    private:
        Texture2D playerIdleTexture;
        Texture2D playerWalkTexture;
        Texture2D playerDeathTexture;

        Texture2D seekerIdleTexture;
        Texture2D seekerWalkTexture;
        Texture2D seekerDeathTexture;

        PlayerAnimation playerAnim[4];

        Camera2D camera;
        player myPlayer;
        player players[4];
        float gameTimer; 
        Network* network;
        int playerId;
        Tilemap tilemap;
    public:
        InGameScreen(Network* networkClient, int playerId);
        AppState update() override;
        void draw() override;
        ~InGameScreen();
};