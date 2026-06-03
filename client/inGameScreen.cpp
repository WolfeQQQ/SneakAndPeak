#include "inGameScreen.h"
#include <cmath>
#include <vector>

#define UI_STAMINA_BAR_PATH "assets/ui/StaminaBar.png"
#define UI_STAMINA_FILL_PATH "assets/ui/StaminaBarFill.png"


// Constructor of a main game screen, initializes textures, tilemaps, shaders etc.
InGameScreen::InGameScreen(Network* networkClient, int playerId) {

    network = networkClient;    // Store the network object for later use
    this->playerId = playerId;  // Keeps track of the client's player ID to manage camera tracking

    // Camera initialization.
    camera = { 0 }; 
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){VIRTUAL_WIDTH/2.0f, VIRTUAL_HEIGHT/2.0f};
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
    canvas = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    lightMask = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    // Fix for the first frame blink
    BeginTextureMode(lightMask); 
        ClearBackground((Color){ 30, 30, 40, 255 }); 
    EndTextureMode();

    // Load the vision shader
    visionShader = LoadShader(0, VISION_SHADER_PATH);

    // Get uniform locations for the shader parameters
    playerPosLoc = GetShaderLocation(visionShader, "playerPos");
    ResolutionLoc = GetShaderLocation(visionShader, "resolution");
    radiusLoc = GetShaderLocation(visionShader, "radius");
    softnessLoc = GetShaderLocation(visionShader, "softness");

    float resolution[2] = {(float)VIRTUAL_WIDTH, (float)VIRTUAL_HEIGHT};
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
    input.up = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
    input.down = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
    input.left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    input.right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

    if(currentGameState != GameState::LOBBY){
    input.shift = IsKeyDown(KEY_LEFT_SHIFT);
    }
    //input.eKey = IsKeyDown(KEY_E);
    
    network ->SendInput(input); // Send the player's input to the server

    GameStatePacket packet; // Structure to hold the received game state from the server

    // Retrieve the current game state from the server
    if(network ->ReceiveState(packet)){
        for(int i = 0; i < MAX_CLIENTS; i++){
            players[i] = packet.players[i];
        }
        gameTimer = packet.timer;
        
        currentGameState = packet.stage;
        if (currentGameState == GameState::GAME_OVER) {
            return AppState::GAME_OVER;
        }
        
        if (previousGameState != currentGameState){
            if(currentGameState == GameState::COUNTDOWN){
                showReveal = true;
                revealTimer = 4.0f;
            }

            if(currentGameState == GameState::GAME){
                bannerText = "THE SEEKER HAS BEEN RELEASED";
                bannerTimer = 3.0f;
            }
        }
        previousGameState = currentGameState;
    }

    // Update player animations based on the received game state
    for(int i = 0; i< MAX_CLIENTS; i++){
        if(!players[i].getIsConnected()) continue;

        if(players[i].getId() == this ->playerId){
            myPlayer = players[i]; // Update the local copy of the client's player
        }
        if(players[i].getIsSeeker()){
            seeker = players[i];
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

    if(showReveal){
        revealTimer -= GetFrameTime();
        if(revealTimer <= 0.0f){
            showReveal = false;
            revealTimer = 0.0f;
        }
    }
    if (bannerTimer > 0){
        bannerTimer -= GetFrameTime();
        if(bannerTimer <= 0.0f){
            bannerTimer = 0.0f;
            bannerText = "";
        }
    }

    if(wasUsingAbility && !myPlayer.getIsViewing()){
        abilityCooldown = 10.0f;
    }

    if (abilityCooldown > 0.0f) {
            abilityCooldown -= GetFrameTime();
            if (abilityCooldown < 0.0f) abilityCooldown = 0.0f; 
    }

    wasUsingAbility = myPlayer.getIsViewing() ? true : false;
    
    camera.target = (Vector2){std::round(myPlayer.getX()) + PLAYER_WIDTH/2, std::round(myPlayer.getY())+PLAYER_LENGTH/2}; // Update the camera target

    BeginTextureMode(lightMask);

        ClearBackground((Color){ 30, 30, 40, 255 }); // Clears the BG with a dark color to create the shadow effect

        BeginMode2D(camera);
            Vector2 center = {myPlayer.getX() + PLAYER_WIDTH/2, myPlayer.getY() + PLAYER_LENGTH/2};
          
            std::vector<Vector2> points;    // Actual raycasted points

            // Cast Rays in a full circle around the player
            for(int i=0 ; i<360; i++){ 
                float rad = i * DEG2RAD;
                float x = cos(rad);
                float y = sin(rad);

                Vector2 hit = center; // Initialize the hit point to the center of the player

                // Cast a ray in the current direction until it hits a wall or go beyond the vision radius
                for (float j = 0; j < VISION_RADIUS; j += 1.0f) {
                    hit.x = center.x + x * j;
                    hit.y = center.y + y * j;

                    if(tilemap.isSolid(hit.x, hit.y)){
                        hit.x += x ; // Move the hit point slightly into the wall
                        hit.y += y ;
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

        EndMode2D();
    EndTextureMode();

    return AppState::IN_GAME; 
}

// Renders whole game screen
void InGameScreen::draw(){

    
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
            float dY = (players[i].getY() + 1) - (drawSize / 2) + PLAYER_LENGTH/2;
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

    float playerPos[2] = {screenPlayerPos.x, (float)VIRTUAL_HEIGHT - screenPlayerPos.y};
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


    if(!myPlayer.getIsSeeker() && myPlayer.getIsViewing() ){
        float angle = atan2(seeker.getY() - myPlayer.getY(), seeker.getX() - myPlayer.getX());
        float rad = 30.0f;

        Vector2 arrowCenter = { 
            myPlayer.getX() + PLAYER_WIDTH/2 + cosf(angle) * rad, 
            myPlayer.getY() + PLAYER_LENGTH/2 + sinf(angle) * rad 
        };
            
        float arrowSize = 8.0f;
        Vector2 p1 = { arrowCenter.x + cosf(angle) * arrowSize, arrowCenter.y + sinf(angle) * arrowSize }; 
        Vector2 p2 = { arrowCenter.x + cosf(angle + 2.5f) * arrowSize, arrowCenter.y + sinf(angle + 2.5f) * arrowSize }; 
        Vector2 p3 = { arrowCenter.x + cosf(angle - 2.5f) * arrowSize, arrowCenter.y + sinf(angle - 2.5f) * arrowSize };
            
        p1 = GetWorldToScreen2D(p1,camera);
        p2 = GetWorldToScreen2D(p2,camera);
        p3 = GetWorldToScreen2D(p3,camera);

        float outline = 3.0f;
        DrawLineEx(p1, p3, outline, BLACK);
        DrawLineEx(p3, p2, outline, BLACK);
        DrawLineEx(p2, p1, outline, BLACK);
        DrawTriangle(p1, p3, p2, RED);
    }

    // Draw the UI elements such as stamina bar and game timer
    if(currentGameState != GameState::LOBBY){ 
        // Stamina bar
        if(myPlayer.getIsSeeker()){
            Color barColor = LIME;

            if (myPlayer.getStamina() <= 60) barColor = ORANGE; 
            if (myPlayer.getStamina() <= 30) {                  
                barColor = RED;
                float pulse = 0.6f + (sin(GetTime() * 15.0f) * 0.4f);
                barColor = Fade(RED, pulse);
            }

            int barHeight = VIRTUAL_HEIGHT - 200;
            
            float percentage = myPlayer.getStamina() /100;
            int fill = (int)(barHeight * percentage);
            int fillY = 100 + (barHeight - fill); 
        
            DrawRectangle(20, 100, 20, barHeight, BLACK);
            DrawRectangle(20, fillY, 20, fill, barColor);
            DrawRectangleLinesEx({ 20, 100, 20, (float)barHeight }, 2, barColor);
        }
        else{
            int barHeight = VIRTUAL_HEIGHT - 200;
            
            float percentage = (10.0f - abilityCooldown) / 10.0f;
            if (percentage < 0.0f) percentage = 0.0f;
            if (percentage > 1.0f) percentage = 1.0f;

            Color barColor = WHITE;
            Color outlineColor = LIGHTGRAY;

            if (myPlayer.getIsViewing()) {
                percentage = 0.0f; 
                float pulse = 0.6f + (sin(GetTime() * 15.0f) * 0.4f);
                outlineColor = Fade(RED, pulse); 
            }
            else if (abilityCooldown <= 0.0f) {
                float pulse = 0.6f + (sin(GetTime() * 15.0f) * 0.4f);
                barColor = Fade(LIME, pulse);
                outlineColor = barColor; 
            }

            int fill = (int)(barHeight * percentage);
            int fillY = 100 + (barHeight - fill); 
        
            DrawRectangle(20, 100, 20, barHeight, BLACK);
            DrawRectangle(20, fillY, 20, fill, barColor);
            DrawRectangleLinesEx({ 20, 100, 20, (float)barHeight }, 2, outlineColor);
        }

    }
    
    // Show the current objective
    const char* objectiveText = "";
    Color objectiveColor = LIGHTGRAY;
    if(currentGameState == GameState::LOBBY){
        objectiveText = "WAITING FOR PLAYERS...";
    }
    else if (currentGameState == GameState::COUNTDOWN){
        objectiveText = "MATCH STARTING...";
    }
    else if (currentGameState == GameState::GAME){
            objectiveText = myPlayer.getIsSeeker() ? "OBJECTIVE: HUNT THEM ALL" : "OBJECTIVE: SURVIVE";
            objectiveColor = myPlayer.getIsSeeker() ? RED : SKYBLUE;
    }

    if(myPlayer.getPlayerState() != player::PlayerState::DEATH){
        if(objectiveText[0] != '\0'){ 
            DrawText(objectiveText, VIRTUAL_WIDTH/2 - MeasureText(objectiveText, 30)/2 + 3, VIRTUAL_HEIGHT - 50 + 3, 30, Fade(BLACK, 0.7f));
            float blink = 0.75f + (sin(GetTime() * 4.0f) *0.25f);
            DrawText(objectiveText, VIRTUAL_WIDTH/2 - MeasureText(objectiveText, 30)/2, VIRTUAL_HEIGHT - 50, 30, Fade(objectiveColor, blink));
        }
    }

    // Show dynamic banners
    if(bannerTimer > 0){
        Color bannerColor = RED;
        float alpha = 1.0f;
        if (bannerTimer < 1.0f){
            alpha = bannerTimer;
        }
        DrawRectangle(0, VIRTUAL_HEIGHT/2 - 120, VIRTUAL_WIDTH, 120, Fade(BLACK, 0.8f * alpha));
        
        DrawText(bannerText.c_str(), VIRTUAL_WIDTH/2 - MeasureText(bannerText.c_str(), 60)/2 + 4, VIRTUAL_HEIGHT/2 - 85 + 4, 60, Fade(BLACK, alpha));
        DrawText(bannerText.c_str(),VIRTUAL_WIDTH/2 - MeasureText(bannerText.c_str(), 60)/2 ,VIRTUAL_HEIGHT/2 - 85, 60, Fade(bannerColor, alpha));
    }

    // New Clock Logic
    int remainingTime = (int) gameTimer;
    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;
    const char* timeText = TextFormat("%0d:%02d",minutes, seconds);
    Color timeColor = WHITE;
    int fontSize = 50;
    if(gameTimer <= 20 && currentGameState != GameState::LOBBY){
        timeColor = RED;
        float blink = gameTimer - remainingTime;
        fontSize += (int)(20.0f * blink);
    }
    DrawText(timeText,VIRTUAL_WIDTH/2 - MeasureText(timeText, fontSize)/2, 40, fontSize, timeColor);

    // Show the client's fps
    DrawFPS(VIRTUAL_WIDTH - 90, 10);

    if (showReveal) {
        float alpha = 1.0f;
        float fadeDuration = 0.5f;
        float maxTimer = 4.0f;

        if (revealTimer < fadeDuration) {
            alpha = revealTimer / fadeDuration;
        }

        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 1.0f) alpha = 1.0f;

        DrawRectangle(0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, Fade(BLACK, 0.925f * alpha));
        
        bool amISeeker = myPlayer.getIsSeeker();

        const char* roleText = amISeeker ? "YOU ARE THE SEEKER" : "YOU ARE A HIDER";
        Color roleColor = amISeeker ? RED : SKYBLUE;

        const char* subText = amISeeker ? "Hunt them down before the time runs out!" : "Blend in and survive!";
        
        int textWidth = MeasureText(roleText, 80);
        int subWidth = MeasureText(subText, 30);
        
        DrawText(roleText, VIRTUAL_WIDTH/2 - textWidth/2, VIRTUAL_HEIGHT/2 - 120, 80, Fade(roleColor, alpha));
        DrawText(subText, VIRTUAL_WIDTH/2 - subWidth/2, VIRTUAL_HEIGHT/2 - 30, 30, Fade(LIGHTGRAY, alpha));
        
        Texture2D roleTex = amISeeker ? seekerIdleTexture : playerIdleTexture;
        Rectangle source = {0, 0, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE};

        Rectangle dest = { (float)VIRTUAL_WIDTH/2 - 64, (float)VIRTUAL_HEIGHT/2 + 20, 128, 128 }; 
        DrawTexturePro(roleTex, source, dest, (Vector2){0,0}, 0.0f, Fade(WHITE, alpha));
    }

   
}