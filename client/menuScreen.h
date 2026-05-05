#pragma once
#include "screen.h"
#include "raylib.h"

class MenuScreen : public Screen {
    private: 
        int optionsCount = 2; // Number of menu options
        int selectedOption = 0; // Variable to track the currently selected menu option

    public:
        AppState update() override;
        void draw() override;
};