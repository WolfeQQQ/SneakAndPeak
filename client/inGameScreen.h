#pragma once
#include "screen.h"
#include "raylib.h"

class InGameScreen : public Screen {
    private:
        int x; //temp
        int y;

    public:
        AppState update() override;
        void draw() override;
    
};