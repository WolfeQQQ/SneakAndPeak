#pragma once
#include "screen.h"
#include "raylib.h"

/**
 * @class MenuScreen
 * @brief Represents the main menu screen of the game, allowing the player to start the game or exit.
 */
class MenuScreen : public Screen {
    private: 
        int optionsCount = 2; // Number of menu options
        int selectedOption = 0; // Variable to track the currently selected menu option

    public:

        /**
         * @brief Updates the menu screen based on user input, allowing navigation through options and selection.
         * @return The next AppState to transition to 
         */
        AppState update() override;

        /**
         * @brief Draws the menu screen.
         */
        void draw() override;
};