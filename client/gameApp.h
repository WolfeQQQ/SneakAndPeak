#pragma once

#include "appState.h"
#include "screen.h"
#include <memory>

class GameApp {

    public:
        GameApp();
        ~GameApp();
        void run();

    private:
        AppState currentState; // Initial state of the application
        std::unique_ptr<Screen> currentScreen; // Pointer to the current screen
        void update();
        void draw();
        void changeState(AppState newState); // Function to change the current state

};