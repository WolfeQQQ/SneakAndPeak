#pragma once
#include "appState.h"

/**
 * @class Screen
 * @brief Abstract class representing a single displayed screen like main menu, lobby, in-game, etc.
 * * All specific screens must inherit from this class.
 */
class Screen {
    public:
        virtual ~Screen() = default; // Virtual destructor 
        virtual AppState update() = 0; // unction to update the screen, returns the next screen to transition 
        virtual void draw() = 0; // Pure virtual function to draw the screen

};