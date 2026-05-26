#pragma once
#include "screen.h"
#include "raylib.h"
#include "../shared/player.h"
#include "tilemap.h"
/**
 * @struct MenuAnimation
 * @brief Holds the animation state for the main menu
 */
struct MenuAnimation{
    int frame = 0;
    float frameTimer = 0.0f;
};

/**
 * @class MenuScreen
 * @brief Represents the main menu screen of the game, allowing the player to start the game or exit.
 */
class MenuScreen : public Screen {
    private: 
        int optionsCount = 3; // Number of menu options
        int selectedOption = 0; // Variable to track the currently selected menu option

        Texture2D hiderWalkTexture; // Texture for the hider's walking animation in the menu
        Camera2D camera; // Camera for the menu screen, used to create a dynamic background effect
        Tilemap menuMap; // Mini map for the menu animation

        player menuPlayer;
        MenuAnimation playerAnim;
        float sceneTimer;

        //animation
        std::vector<Vector2> waypoints;
        size_t currentWaypoint = 0;

        RenderTexture2D lightMask; // Render texture for the light mask, used to create a dynamic lighting effect in the menu background
        Shader visionShader; // Shader for the lighting effect in the menu background

        int playerPosLoc; // Uniform location for the player's position in the vision shader
        int ResolutionLoc; // Uniform location for the screen resolution in the vision shader
        int radiusLoc; // Uniform location for the vision radius in the vision shader
        int softnessLoc; // Uniform location for the vision softness in the vision shader

    public:

        MenuScreen();

        ~MenuScreen();

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