#pragma once

#include "contstants.h"

enum class GameState {
    LOBBY,
    COUNTDOWN,
    GAME,
    GAME_OVER
};

struct GameStatePacket {
    GameState stage;
    float timer;
    player players[MAX_CLIENTS];
};