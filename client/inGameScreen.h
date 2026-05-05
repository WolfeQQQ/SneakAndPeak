#pragma once
#include "screen.h"
#include "../shared/player.h"
#include "raylib.h"

class InGameScreen : public Screen {
    private:
        Camera2D camera;
        player myPlayer;
    public:
        InGameScreen();
        AppState update() override;
        void draw() override;
    
};