#pragma once

/**
 * @enum AppState
 * @brief Defines the different states of the client application, 
 * used to manage the flow of the application and determine which screen should be displayed.
 * 
 */
enum class AppState{
    MAIN_MENU,  // Initial Main menu screen
    CONNECTING, // Screen "shown" while connecting to the server
    SETTINGS, // Kinda self-explanatory
    LOBBY,  // Screen that's used to enter server's ip-address and port.
    IN_GAME,    // Main game screen where gameplay occurs
    TUTORIAL,
    DISCONNECTED,
    GAME_OVER,  // End game screen showing a summary of the game
    EXIT
};