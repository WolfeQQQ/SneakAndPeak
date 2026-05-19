#pragma once
#include "screen.h"
#include "../shared/player.h"
#include "network.h"
#include "raylib.h"
#include "tilemap.h"


enum class PlayerState {
    IDLE,
    WALK,
    DEATH
};      //TEMP TO BE IMPLEMENTED IN SERVER
enum class PlayerDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT
};      //TEMP TO BE IMPLEMENTED IN SERVER

struct PlayerAnimation {
    PlayerState lastState = PlayerState::IDLE;
    PlayerState state = PlayerState::IDLE;
    PlayerDirection direction = PlayerDirection::DOWN;

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
        Network* network;
        int playerId;
        Tilemap tilemap;
    public:
        InGameScreen(Network* networkClient, int playerId);
        AppState update() override;
        void draw() override;
        ~InGameScreen();
};