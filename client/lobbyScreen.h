#pragma once
#include "screen.h"
#include "raylib.h"
#include "tilemap.h"
#include <string>

/**
 * @class LobbyScreen
 * @brief Represents the lobby that allows players to connect to the servers
 */
class LobbyScreen : public Screen {
    private:
        Tilemap bgMap;
        Shader visionShader; 
        RenderTexture2D lightMask;
        Camera2D camera;

        // Shader stuff
        int playerPosLoc;
        int resolutionLoc;
        int radiusLoc;
        int softnessLoc;

        std::string ipInput = "127.0.0.1";
        std::string portInput = "5000";

        int active = 0;
        int frameCounter = 0;

    public:
        LobbyScreen();

        ~LobbyScreen();

        /**
         * @brief Updates the lobby screen based on user input, allowing joining to the game.
         * @return The next AppState to transition to 
         */
        AppState update() override;

        /**
         * @brief Draws the menu screen.
         */
        void draw() override;

        // Getters for the GameApp class
        std::string getIP() const { return ipInput; }
        int getPort() const { return std::stoi(portInput); }


};