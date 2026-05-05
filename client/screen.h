#pragma once
#include "appState.h"

class Screen {
    public:
        virtual ~Screen() = default; // Virtual destructor 
        virtual AppState update() = 0; // unction to update the screen, returns the next screen to transition 
        virtual void draw() = 0; // Pure virtual function to draw the screen

};