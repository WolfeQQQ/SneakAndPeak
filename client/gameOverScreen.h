#pragma once
#include "screen.h"
#include "network.h"
#include "raylib.h"
#include "tilemap.h"
#include "../shared/gameState.h"

/**
 * @class GameOverScreen
 * @brief Represents the last screen of the game, allows the player to exit the game or retry
 */
class GameOverScreen : public Screen {
private:
    Network* network;
    GameStatePacket lastPacket; 
    bool backToLobbyPressed;

    Tilemap bgMap; // Map for the background looks;
    Shader visionShader; 
    RenderTexture2D lightMask;
    Camera2D camera;

    int playerPosLoc;
    int resolutionLoc;
    int radiusLoc;
    int softnessLoc;

    const int optionsCount = 2;
    int selectedOption = 0;

public:
    GameOverScreen(Network* networkClient, const GameStatePacket& finalPacket);
    
    /**
    * @brief Updates the screen based on user input, allowing navigation through options.
    * @return The next AppState to transition to 
    */
    AppState update() override;

    /**
     * @brief Draws the settings screen.
     */
    void draw() override;
    ~GameOverScreen();
};