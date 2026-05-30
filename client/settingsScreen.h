#pragma once
#include "screen.h"
#include "raylib.h"
#include "tilemap.h"
#include <vector>
#include <string>

/**
 * @class SettingsScreen
 * @brief Represents the settings screen of the game, allowing the player to change resolution etc.
 */
class SettingsScreen : public Screen {
    private: 
        std::vector<std::string> options; 
        int selectedOption = 0; // Variable to track the currently selected menu option
        bool isFullscreen = false;

    public:

        SettingsScreen();

        ~SettingsScreen();

        /**
         * @brief Updates the settings screen based on user input, allowing navigation through options and changing them.
         * @return The next AppState to transition to 
         */
        AppState update() override;

        /**
         * @brief Draws the settings screen.
         */
        void draw() override;
};