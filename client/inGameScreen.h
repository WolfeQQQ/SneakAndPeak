#pragma once
#include "screen.h"
#include "../shared/player.h"
#include "network.h"
#include "raylib.h"
#include "tilemap.h"


/**
 * @struct PlayerAnimation
 * @brief Struct for managing player animation states and frame timing
 */
struct PlayerAnimation {
    player::PlayerState lastState = player::PlayerState::IDLE;  // Previous state to detect transitions
    player::PlayerState state = player::PlayerState::IDLE;  // Current player state (IDLE, WALK, DEATH)
    player::Direction direction = player::Direction::DOWN;  // Current player direction (DOWN, UP, LEFT, RIGHT)

    int frame = 0;  // Current animation frame index
    float frameTimer = 0.0f;    // Timer to control animation speed

};

/**
 *@class InGameScreen
 *@brief Represents the main game screen where gameplay occurs
 * * Handles rendering the tilemap, processing local input, and updating player states/positions based on server updates.
 */
class InGameScreen : public Screen {
    private:
        //Textures: Hider - Idle, Walk and Death
        Texture2D playerIdleTexture;
        Texture2D playerWalkTexture;
        Texture2D playerDeathTexture;

        //Textures: Seeker - Idle, Walk and Death
        Texture2D seekerIdleTexture;
        Texture2D seekerWalkTexture;
        Texture2D seekerDeathTexture;

        
        PlayerAnimation playerAnim[4];  // All players animation states
        Camera2D camera;    // Raylib camera used to track the player's location
        player myPlayer;    // Local copy of client's player data. Used for camera tracking
        player players[4];  // Array to hold the state of all players
        float gameTimer;    // Global game timer, received from the server
        Network* network;   // Pointer to the network object
        int playerId;       // Unique indentifier of the client's player
        Tilemap tilemap;    // Tilemap object to manage loading and rendering the tilemap

        RenderTexture2D canvas;     // Render texture for the main game view, used for post-processing effects
        RenderTexture2D lightMask;  // Render texture for the light mask, used to make shadows with raycasting

        Shader visionShader;    // Post processing shader, used make the player's vision pretty ;))

        // Shader uniform locations
        int playerPosLoc;   
        int ResolutionLoc;  
        int radiusLoc;      
        int softnessLoc;    

    public:
        /**
         * @brief Constructor for the InGameScreen class, initializes textures, tilemap, camera and shader.
         * @param networkClient Pointer to the Network object for communication with the server.
         * @param playerId Unique identifier of the client's player.
         */
        InGameScreen(Network* networkClient, int playerId);

        /**
         * @brief Updates the game state by processing player input, sending it to the server, and receiving updated game state.
         * @return The next AppState to transition to
         */
        AppState update() override;

        /**
         * @brief Renders the game screen, including the tilemap, players, shaders and UI elements.
         */
        void draw() override;

        /**
         * @brief Destructor, unloads textures, tilemap and shader resources.
         */
        ~InGameScreen();
};