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
        int selectedOption = 0; // Variable to track the currently selected menu option
        const int optionsCount = 3; // resolution / fullscreen / exit

        std::vector<Vector2> resolutionValues = {{1280,720},{1600,900},{1920,1080},{2560,1440}}; // List of available resolutions
        int currentResolution = 0; // Currrent resolution index

        std::vector<std::string> fullscreenValues ={"NUH UH", "HELL YEAH"};
        int currentFullscreen = 0; 
        Tilemap bgMap; // Map for the background looks;
        Shader visionShader; 
        RenderTexture2D lightMask;
        Camera2D camera;

        // Shader stuff
        int playerPosLoc;
        int resolutionLoc;
        int radiusLoc;
        int softnessLoc;

        void applyResolution();
        void applyFullscreen();


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