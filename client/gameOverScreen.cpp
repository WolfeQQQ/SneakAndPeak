#include "gameOverScreen.h"
#include <cmath>
#include <string>

GameOverScreen::GameOverScreen(Network * clientNetwork, const GameStatePacket& finalPacket){
    network = clientNetwork;
    lastPacket = finalPacket;
    backToLobbyPressed = false;

    bgMap.load(MENU_CSV_PATH, TILESET_PATH, TILE_SIZE);

    visionShader = LoadShader(0, VISION_SHADER_PATH);
    playerPosLoc = GetShaderLocation(visionShader, "playerPos");
    resolutionLoc = GetShaderLocation(visionShader, "resolution");
    radiusLoc = GetShaderLocation(visionShader, "radius");
    softnessLoc = GetShaderLocation(visionShader, "softness");

    lightMask = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
     
    BeginTextureMode(lightMask); 
        ClearBackground((Color){ 30, 30, 40, 255 }); 
    EndTextureMode();

    camera = { 0 };
    camera.target = (Vector2){10.0f * TILE_SIZE, 10.0f *TILE_SIZE}; // Lock the camera for better appearance
    camera.offset = (Vector2){VIRTUAL_WIDTH/2.0f, VIRTUAL_HEIGHT/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = CAMERA_ZOOM;

}

GameOverScreen::~GameOverScreen() {
    bgMap.unload();
    UnloadShader(visionShader);
    UnloadRenderTexture(lightMask); 
}

AppState GameOverScreen::update(){
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
        selectedOption++;
        if (selectedOption >= optionsCount) {
            selectedOption = 0; 
        }
    }
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        selectedOption--;
        if (selectedOption < 0) {
            selectedOption = optionsCount - 1; 
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (selectedOption == 0 && !backToLobbyPressed) {
            player::ClientInput input;
            input.up = false; input.down = false; input.left = false; input.right = false; input.shift = false;
            input.backToLobby = true;

            // Wysyłamy żądanie do serwera
            if (network != nullptr) {
                network->SendInput(input);
            }
            backToLobbyPressed = true;
        } 
        else if (selectedOption == 1) {
            return AppState::MAIN_MENU;
        }
    }
    GameStatePacket packet;
    if (network != nullptr) {

        network->ReceiveState(packet);
        

        if (!network->IsConnected()) {
            return AppState::CONNECTING;
        }
    }

    BeginTextureMode(lightMask);
        ClearBackground((Color){ 30, 30, 40, 255 }); 
        DrawCircleGradient(Vector2{VIRTUAL_WIDTH + 175, VIRTUAL_HEIGHT}, 1500, WHITE, (Color){ 30, 30, 40, 255 });
    EndTextureMode();

    return AppState::GAME_OVER;
}

void GameOverScreen::draw() {

    BeginMode2D(camera);
        bgMap.draw();
    EndMode2D();

    float centerPos[2] = { (float)VIRTUAL_WIDTH/2, (float)VIRTUAL_HEIGHT/2 };
    float resolution[2] = { (float)VIRTUAL_WIDTH, (float)VIRTUAL_HEIGHT };
    float radius = VISION_RADIUS;
    float softness = VISION_SOFTNESS + 200; 

    SetShaderValue(visionShader, playerPosLoc, centerPos, SHADER_UNIFORM_VEC2);
    SetShaderValue(visionShader, resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(visionShader, radiusLoc, &radius, SHADER_UNIFORM_FLOAT);
    SetShaderValue(visionShader, softnessLoc, &softness, SHADER_UNIFORM_FLOAT);

    BeginBlendMode(BLEND_MULTIPLIED);
        BeginShaderMode(visionShader);
            Rectangle src = { 0.0f, 0.0f, (float)lightMask.texture.width, -(float)lightMask.texture.height };
            DrawTextureRec(lightMask.texture, src, (Vector2){ 0, 0 }, WHITE);
        EndShaderMode();
    EndBlendMode();


    int screenWidth = VIRTUAL_WIDTH;
    int screenHeight = VIRTUAL_HEIGHT;

    float pulse = sin(GetTime() * 3.0f) * 0.15f + 0.85f;
    
    std::string titleText = "GAME OVER";
    std::string descText = "";
    Color teamColor = WHITE;

    if (lastPacket.victoryType == 1) {
        titleText = "SEEKER WON";
        descText = "All hiders were found and caught";
        teamColor = (Color){ 220, 40, 40, (unsigned char)(255 * pulse) }; 
    } else if (lastPacket.victoryType == 2) {
        titleText = "HIDERS WON";
        descText = "Time has passed. You have managed to survive in the darkness.";
        teamColor = (Color){ 20, 20, 255, (unsigned char)(255 * pulse) }; 
    }

    int titleFontSize = 50;
    int titleWidth = MeasureText(titleText.c_str(), titleFontSize);
    DrawText(titleText.c_str(), screenWidth / 2 - titleWidth / 2, screenHeight / 4, titleFontSize, teamColor);

    int descFontSize = 20;
    int descWidth = MeasureText(descText.c_str(), descFontSize);
    DrawText(descText.c_str(), screenWidth / 2 - descWidth / 2, screenHeight / 4 + 70, descFontSize, WHITE);
    int buttonWidth = 250;
    int buttonHeight = 50;
    int buttonX = screenWidth / 2 - buttonWidth / 2; 


    int buttonY = screenHeight / 2 + 30;
    Color lobbyColor = (selectedOption == 0) ? WHITE : DARKGRAY;
    Color lobbyTextColor = (selectedOption == 0) ? BLACK : WHITE;

    DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, lobbyColor);

    std::string lobbyText = backToLobbyPressed ? "WAITING..." : "BACK TO LOBBY";
    int lobbyTextWidth = MeasureText(lobbyText.c_str(), 20);
    DrawText(lobbyText.c_str(), buttonX + (buttonWidth / 2 - lobbyTextWidth / 2), buttonY + (buttonHeight / 2 - 10), 20, lobbyTextColor);


    int buttonExitY = buttonY + 70;


    Color exitColor = (selectedOption == 1) ? WHITE : DARKGRAY;
    Color exitTextColor = (selectedOption == 1) ? BLACK : WHITE;
    DrawRectangle(buttonX, buttonExitY, buttonWidth, buttonHeight, exitColor);

    std::string exitText = "BACK TO MENU";
    int exitTextWidth = MeasureText(exitText.c_str(), 20);
    DrawText(exitText.c_str(), buttonX + (buttonWidth / 2 - exitTextWidth / 2), buttonExitY + (buttonHeight / 2 - 10), 20, exitTextColor);
}
