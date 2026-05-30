#pragma once

#include "appState.h"
#include "screen.h"
#include <memory>
#include "network.h"

/** @class GameApp
 * @brief Main application class that manages the game loop and state transitions.
 */
class GameApp {

    public:
        GameApp(); // Constructor, initializes the application
        ~GameApp(); // Destructor, cleans up resources
        void run(); // Main game loop, handles updating and drawing

    private:
        AppState currentState; // Initial state of the application
        std::unique_ptr<Screen> currentScreen; // Pointer to the current screen
        void update();  // Function to update the application state
        void draw();
        void changeState(AppState newState); // Function to change the current state
        Network networkClient; // Handles communication with the game server
        int playerId; // Unique identifier for the player, assigned by the server
        GameStatePacket lastPacket;
};