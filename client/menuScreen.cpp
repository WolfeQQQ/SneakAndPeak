#include "menuScreen.h"
#include <cmath>
#include <string>
#include <vector>
#include "../shared/contstants.h"
#include "raylib.h"


//Constructor initializes textures, shaders etc. for the menu screen
MenuScreen::MenuScreen() {
    // Load textures for the animated menu
    hiderWalkTexture = LoadTexture(PLAYER_WALK_PATH);

    // Load the tilemap
    menuMap.load(MENU_CSV_PATH, TILESET_PATH, TILE_SIZE);

    // Prepare the canvas for dynamic lighting effect
    lightMask = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    // Fix for the first frame blink
    BeginTextureMode(lightMask); 
        ClearBackground((Color){ 30, 30, 40, 255 }); 
    EndTextureMode();

    // Prepare the shader for the dynamic lighting effect 
    visionShader = LoadShader(0, VISION_SHADER_PATH);
    playerPosLoc = GetShaderLocation(visionShader, "playerPos");
    ResolutionLoc = GetShaderLocation(visionShader, "resolution");
    radiusLoc = GetShaderLocation(visionShader, "radius");
    softnessLoc = GetShaderLocation(visionShader, "softness");

    float resolution[2] = {(float)VIRTUAL_WIDTH, (float)VIRTUAL_HEIGHT};
    SetShaderValue(visionShader, ResolutionLoc, resolution, SHADER_UNIFORM_VEC2); // Set the resolution uniform in the shader because it doesn't change

    camera = { 0 };
    camera.target = (Vector2){10.0f * TILE_SIZE, 10.0f *TILE_SIZE}; // Lock the camera for better appearance
    camera.offset = (Vector2){VIRTUAL_WIDTH/2.0f, VIRTUAL_HEIGHT/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = CAMERA_ZOOM;

    std::vector<Vector2> gridWaypoints =  {
        {20.0f, 5.0f}, 
        {17.0f, 5.0f}, 
        {17.0f, 9.0f}, 
        {16.0f, 9.0f},
        {16.0f, 5.0f},
        {11.0f, 5.0f}, 
        {11.0f, 14.0f}, 
        {15.0f, 14.0f}, 
        {15.0f, 12.0f},
        {20.0f, 12.0f}   
    };

    // Translate the grid waypoints to World coordinates
    for(const auto& point: gridWaypoints){
        waypoints.push_back({ (point.x * TILE_SIZE) + (TILE_SIZE/4), (point.y * TILE_SIZE)  });
    }
    
    // Set the player coordinates to the first waypoint
    int startPos = rand() % gridWaypoints.size();
    currentWaypoint = startPos;
    menuPlayer.setX(waypoints[startPos].x);
    menuPlayer.setY(waypoints[startPos].y);

    menuPlayer.setDirection(player::Direction::LEFT);
    menuPlayer.setPlayerState(player::PlayerState::WALK);

}

// Destructor, unloads everything that was loaded in the constructor
MenuScreen::~MenuScreen() {
    UnloadTexture(hiderWalkTexture);
    menuMap.unload();
    UnloadRenderTexture(lightMask);
    UnloadShader(visionShader);
}


// Whole menu update logic. Handles navigation and selection of options. Returns the next state to transition to.
// UPDATE: Now also handles the animations
AppState MenuScreen::update(){

    // Input Handling
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
        selectedOption = (selectedOption - 1 < 0) ? optionsCount - 1 : selectedOption - 1;
    }
    if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
        selectedOption = (selectedOption + 1) % optionsCount;
    }

    if (IsKeyPressed(KEY_ENTER)){
        switch (selectedOption){
            case 0:
                return AppState::LOBBY;
                break;
            case 1:   
                return AppState::EXIT;      
                break;
            case 2: 
                return AppState::SETTINGS;
                break;
        }
    }

    // Animations update
    float deltaTime = GetFrameTime();
    playerAnim.frameTimer += deltaTime;
    sceneTimer += deltaTime;

    if (playerAnim.frameTimer >= ANIMATION_SPEED){
        playerAnim.frameTimer = 0.0f;
        playerAnim.frame = (playerAnim.frame + 1) % WALK_FRAMES;
    }

    Vector2 target = waypoints[currentWaypoint];
    float playerX = menuPlayer.getX();
    float playerY = menuPlayer.getY();

    float dx = target.x - playerX;
    float dy = target.y - playerY;

    float distance = sqrtf(dx*dx + dy*dy);

    if (distance < 2.0f){
        currentWaypoint = (currentWaypoint + 1) % waypoints.size();
    }
    else{
        float speed = 72.0f;
        float dirX = dx / distance;
        float dirY = dy / distance;

        menuPlayer.setX(playerX + dirX * speed * deltaTime);
        menuPlayer.setY(playerY + dirY * speed * deltaTime);

        if(fabs(dirX) > fabs(dirY)){
            if (dirX > 0){
                menuPlayer.setDirection(player::Direction::RIGHT);
            }
            else menuPlayer.setDirection(player::Direction::LEFT);
        }
        else{
            if(dirY > 0){
                menuPlayer.setDirection(player::Direction::DOWN);
            }
            else menuPlayer.setDirection(player::Direction::UP);
        }

    }


    BeginTextureMode(lightMask);

        ClearBackground((Color){ 30, 30, 40, 255 }); 

        BeginMode2D(camera);

             Vector2 center = {menuPlayer.getX() + PLAYER_WIDTH/2, menuPlayer.getY() + PLAYER_LENGTH/2};
          
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

                    if(menuMap.isSolid(hit.x, hit.y)){
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


    return AppState::MAIN_MENU; // Stay on the main menu if no option is selected
}

// Renders the Main menu
void MenuScreen::draw(){

    BeginMode2D(camera);

        menuMap.draw();
        player::Direction direction = menuPlayer.getDirection();
        int row;
        if(direction == player::Direction::DOWN) row = 0;
            else if(direction == player::Direction::LEFT || direction == player::Direction::RIGHT) row = 1;
            else if(direction == player::Direction::UP) row = 2;

        float frameWidth = PLAYER_FRAME_SIZE;
        if(direction == player::Direction::LEFT){
            frameWidth = -PLAYER_FRAME_SIZE; // Flip the texture for left direction
        }

        Rectangle source = {(float)(playerAnim.frame * PLAYER_FRAME_SIZE), (float)(row * PLAYER_FRAME_SIZE), frameWidth, PLAYER_FRAME_SIZE};

        float scale = 2.0f;
        float drawSize = PLAYER_FRAME_SIZE * scale;
        float dX = menuPlayer.getX() - (drawSize / 2) + PLAYER_WIDTH/2;
        float dY = (menuPlayer.getY() + 1) - (drawSize / 2) + PLAYER_LENGTH/2;
        Rectangle dest = {dX, dY, drawSize, drawSize};
        DrawTexturePro(hiderWalkTexture, source, dest, (Vector2){0,0}, 0.0f, WHITE);

    EndMode2D();

    Vector2 wPlayerPos = {menuPlayer.getX()+PLAYER_WIDTH/2, menuPlayer.getY()+PLAYER_LENGTH/2}; // World position of the player, used for shader calculations
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

    //UI ELEMENTS
    //UI ELEMENTS 

    int startX = 100;
    int startY = 200;
    int spacing = 120;

    // Colors for the currently selected and non-selected options
    Color b1 = (selectedOption == 0) ? WHITE : DARKGRAY; 
    Color b2 = (selectedOption == 1) ? WHITE : DARKGRAY; 
    Color b3 = (selectedOption == 2) ? WHITE : DARKGRAY;

    Color c1 = (selectedOption == 0) ? BLACK : WHITE;
    Color c2 = (selectedOption == 1) ? BLACK : WHITE; 
    Color c3 = (selectedOption == 2) ? BLACK : WHITE;

    
    DrawText("SNEAK&PEAK", 100, VIRTUAL_HEIGHT/ 2 - 300, 100, WHITE); // TODO - center text properly
    
    DrawRectangle(100, 200, 300, 75, b1); 
    DrawText("Start Game", 110, 220, 30, c1);   // TODO -  Buttons too...
    
    DrawRectangle(100, 320, 300, 75, b3);
    DrawText("Settings", 110, 340, 30, c3);

    DrawRectangle(100, 440, 300, 75, b2);
    DrawText("Exit", 110, 460, 30, c2);

    //TODO COORDINATES AND SIZE ADJUSTMENTS
}