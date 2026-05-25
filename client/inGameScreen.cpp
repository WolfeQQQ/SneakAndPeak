#include "inGameScreen.h"
#include <cmath>
#include <vector>

// Constructor of a main game screen, initializes textures, tilemaps, shaders etc.
InGameScreen::InGameScreen(Network* networkClient, int playerId) {

    network = networkClient;    // Store the network object for later use
    this->playerId = playerId;  // Keeps track of the client's player ID to manage camera tracking

    // Camera initialization.
    camera = { 0 }; 
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = CAMERA_ZOOM;

    // Load player and seeker textures for different states 
    playerIdleTexture = LoadTexture(PLAYER_IDLE_PATH);
    playerWalkTexture = LoadTexture(PLAYER_WALK_PATH);
    playerDeathTexture = LoadTexture(PLAYER_DEATH_PATH);

    seekerIdleTexture = LoadTexture(SEEKER_IDLE_PATH);
    seekerWalkTexture = LoadTexture(SEEKER_WALK_PATH);
    seekerDeathTexture = LoadTexture(SEEKER_DEATH_PATH);

    // Load the tilemap from the CSV file and the tileset texture
    tilemap.load(MAP_CSV_PATH, TILESET_PATH, TILE_SIZE); 

    // Initialize render textures for the main game view and the raycasted light mask
    canvas = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    lightMask = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    // Load the vision shader
    visionShader = LoadShader(0, VISION_SHADER_PATH);

    // Get uniform locations for the shader parameters
    playerPosLoc = GetShaderLocation(visionShader, "playerPos");
    ResolutionLoc = GetShaderLocation(visionShader, "resolution");
    radiusLoc = GetShaderLocation(visionShader, "radius");
    softnessLoc = GetShaderLocation(visionShader, "softness");

    float resolution[2] = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    SetShaderValue(visionShader, ResolutionLoc, resolution, SHADER_UNIFORM_VEC2); // Set the resolution uniform in the shader because it doesn't change
}

// Destructor of the main game screen, unloads everything :)
InGameScreen::~InGameScreen() {
    UnloadTexture(playerIdleTexture);
    UnloadTexture(playerWalkTexture);
    UnloadTexture(playerDeathTexture);
    UnloadTexture(seekerIdleTexture);
    UnloadTexture(seekerWalkTexture);
    UnloadTexture(seekerDeathTexture);
    tilemap.unload();
    UnloadRenderTexture(canvas);
    UnloadRenderTexture(lightMask);
    UnloadShader(visionShader);
}

// Handles player's input collections, networking and animations updates.
AppState InGameScreen::update(){

    // Initialize a client input structure and gather currenty pressed keys
    player::ClientInput input;
    input.up = IsKeyDown(KEY_W);
    input.down = IsKeyDown(KEY_S);
    input.left = IsKeyDown(KEY_A);
    input.right = IsKeyDown(KEY_D);
    input.shift = IsKeyDown(KEY_LEFT_SHIFT);
    //input.eKey = IsKeyDown(KEY_E);
    
    network ->SendInput(input); // Send the player's input to the server

    GameStatePacket packet; // Structure to hold the received game state from the server

    // Retrieve the current game state from the server
    if(network ->ReceiveState(packet)){
        for(int i = 0; i < MAX_CLIENTS; i++){
            players[i] = packet.players[i];
        }
        gameTimer = packet.timer;
    }

    // Update player animations based on the received game state
    for(int i = 0; i< MAX_CLIENTS; i++){
        if(!players[i].getIsConnected()) continue;

        if(players[i].getId() == this ->playerId){
            myPlayer = players[i]; // Update the local copy of the client's player
        }

        PlayerAnimation& anim = playerAnim[i]; // Get the animation state for the current player
        player::PlayerState currentState = players[i].getPlayerState();

        // If the player's state has changed, reset the animation to the first frame
        if(anim.lastState != currentState){
            anim.frame = 0;
            anim.frameTimer = 0.0f;
            anim.lastState = currentState;
        }

        // Determine the number of frames in the current animation based on the player's state
        int maxFrames = IDLE_FRAMES;
        if(currentState == player::PlayerState::DEATH) maxFrames = DEATH_FRAMES;
        if(currentState == player::PlayerState::WALK) maxFrames = WALK_FRAMES;

        // Update the animation frame based on the timer and animation speed
        anim.frameTimer += GetFrameTime();
        if(anim.frameTimer >= ANIMATION_SPEED){
            anim.frameTimer = 0.0f;

            // If the player died, the animation shouldn't loop
            if(currentState == player::PlayerState::DEATH){
                if(anim.frame < maxFrames - 1){
                    anim.frame++;
                }
            }
            else{
                anim.frame = (anim.frame + 1) % maxFrames; // if not, loops the animation
            }

        }

    }
    
    camera.target = (Vector2){std::round(myPlayer.getX()) + PLAYER_WIDTH/2, std::round(myPlayer.getY())+PLAYER_LENGTH/2}; // Update the camera target

    return AppState::IN_GAME; 
}

// Renders whole game screen
void InGameScreen::draw(){

    BeginTextureMode(lightMask);

        ClearBackground((Color){ 30, 30, 40, 255 }); // Clears the BG with a dark color to create the shadow effect

        BeginMode2D(camera);
            Vector2 center = {myPlayer.getX() + PLAYER_WIDTH/2, myPlayer.getY() + PLAYER_LENGTH/2};
            float radius = VISION_RADIUS;   // Radius of the player's vision, used for raycasting

            std::vector<Vector2> points;    // Actual raycasted points

            // Cast Rays in a full circle around the player
            for(int i=0 ; i<360; i++){ 
                float rad = i * DEG2RAD;
                float x = cos(rad);
                float y = sin(rad);

                Vector2 hit = center; // Initialize the hit point to the center of the player

                // Cast a ray in the current direction until it hits a wall or go beyond the vision radius
                for (float j = 0; j < radius; j += 5.0f) {
                    hit.x = center.x + x * j;
                    hit.y = center.y + y * j;

                    if(tilemap.isSolid(hit.x, hit.y)){
                        break;
                    }
                }
                points.push_back(hit); // Store the raycasted point

            }

            // Draw the vision cone using the raycasted points
            for(size_t i =0; i<points.size()-1; i++){
                DrawTriangle(center, points[i+1], points[i], WHITE);
            }
            DrawTriangle(center, points[0], points.back(), WHITE);

            // Better visual effect :)
            for(size_t i = 0; i < points.size(); i++){
                DrawCircleGradient(points[i], 25.0f, WHITE, (Color){255, 255, 255, 0});
            }

        EndMode2D();
    EndTextureMode();

    // Start of the main game rendering
    BeginMode2D(camera);
        tilemap.draw(); // Draw the tilemap first so players are rendered on top of it
        
        // Draw all players based on their current state and direction
        for(int i = 0; i<MAX_CLIENTS; i++){
            if(!players[i].getIsConnected()) continue;

            PlayerAnimation& anim = playerAnim[i];
            player::PlayerState currentState = players[i].getPlayerState();
        
            player::Direction direction = players[i].getDirection();
           
            Texture2D currentTexture;

            // Determine the correct texture to use 
            switch (currentState)
            {
            case player::PlayerState::IDLE:
                if(players[i].getIsSeeker()) currentTexture = seekerIdleTexture;
                else currentTexture = playerIdleTexture;
                break;
            case player::PlayerState::WALK:
                if(players[i].getIsSeeker()) currentTexture = seekerWalkTexture;
                else currentTexture = playerWalkTexture;
                break;
            case player::PlayerState::DEATH:
                if(players[i].getIsSeeker()) currentTexture = seekerDeathTexture;
                else currentTexture = playerDeathTexture;
                break;
            default:
                currentTexture = playerIdleTexture;
                break;
            }

            // Chose a correct row in the sprite sheet based on the player's direction
            int row; // TODO CHANGE TO CONSTANTS
            if(direction == player::Direction::DOWN) row = 0;
            else if(direction == player::Direction::LEFT || direction == player::Direction::RIGHT) row = 1;
            else if(direction == player::Direction::UP) row = 2;


            float frameWidth = PLAYER_FRAME_SIZE;
            if(direction == player::Direction::LEFT){
                frameWidth = -PLAYER_FRAME_SIZE; // Flip the texture for left direction
            }

            Rectangle source = {(float)(anim.frame * PLAYER_FRAME_SIZE), (float)(row * PLAYER_FRAME_SIZE), frameWidth, PLAYER_FRAME_SIZE};

            // Scale the texture and calculate the destination rectangle for drawing
            float scale = 2.0f;
            float drawSize = PLAYER_FRAME_SIZE * scale;
            float dX = players[i].getX() - (drawSize / 2) + PLAYER_WIDTH/2;
            float dY = players[i].getY() - (drawSize / 2) + PLAYER_LENGTH/2;
            Rectangle dest = {dX, dY, drawSize, drawSize};
            DrawTexturePro(currentTexture, source, dest, (Vector2){0,0}, 0.0f, WHITE);

            // Draw player IDs above their heads for debugging purposes
            if(players[i].getIsConnected()){
                DrawText(std::to_string(players[i].getId()).c_str(), std::round(players[i].getX()), std::round(players[i].getY()) - 20, 10, WHITE);
            }


        }

    EndMode2D();

    Vector2 wPlayerPos = {myPlayer.getX()+PLAYER_WIDTH/2, myPlayer.getY()+PLAYER_LENGTH/2}; // World position of the player, used for shader calculations
    Vector2 screenPlayerPos = GetWorldToScreen2D(wPlayerPos, camera); // Convert the world position to screen coordinates

    float playerPos[2] = {screenPlayerPos.x, screenPlayerPos.y};
    float radiusUniform = VISION_RADIUS;
    float softness = VISION_SOFTNESS;
    
    // Set the shader values for the player's position, vision radius and softness
    SetShaderValue(visionShader, playerPosLoc, playerPos, SHADER_UNIFORM_VEC2);
    SetShaderValue(visionShader, radiusLoc, &radiusUniform, SHADER_UNIFORM_FLOAT);
    SetShaderValue(visionShader, softnessLoc, &softness, SHADER_UNIFORM_FLOAT);

    // Draw the light mask with the vision shader applied to create the shadows
    BeginBlendMode(BLEND_MULTIPLIED);
        BeginShaderMode(visionShader); 
            Rectangle sourceRec = { 0.0f, 0.0f, (float)lightMask.texture.width, -(float)lightMask.texture.height };
            DrawTextureRec(lightMask.texture, sourceRec, (Vector2){ 0, 0 }, WHITE);
        EndShaderMode();
    EndBlendMode();

    // Draw the UI elements such as stamina bar and game timer
    DrawText("Stamina: ", 10, 10, 20,WHITE);
    DrawRectangle(10,40, myPlayer.getStamina() * 2, 20, GREEN);
    DrawRectangleLines(10,40, 200, 20, WHITE);
    DrawText(std::to_string(gameTimer).c_str(),GetScreenWidth()/2 - 25, 20, 20, WHITE);
   
}