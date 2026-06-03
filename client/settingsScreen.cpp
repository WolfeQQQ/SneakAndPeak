#include "settingsScreen.h"
#include "../shared/contstants.h"
#include <string>
#include <cmath>


SettingsScreen :: SettingsScreen(){
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

    currentFullscreen = IsWindowFullscreen() ? 1: 0;
    int currentWidth = GetScreenWidth();
    int currentHeight = GetScreenHeight();
    currentResolution = 0;
    for(size_t i = 0; i < resolutionValues.size(); i++){
        if ((int)resolutionValues[i].x == currentWidth && (int)resolutionValues[i].y == currentHeight) {
            currentResolution = i;
            break;
        }
    }

    camera = { 0 };
    camera.target = (Vector2){10.0f * TILE_SIZE, 10.0f *TILE_SIZE}; // Lock the camera for better appearance
    camera.offset = (Vector2){VIRTUAL_WIDTH/2.0f, VIRTUAL_HEIGHT/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = CAMERA_ZOOM;

}

SettingsScreen :: ~SettingsScreen(){
    bgMap.unload();
    UnloadShader(visionShader);
    UnloadRenderTexture(lightMask); 
}



AppState SettingsScreen::update() {
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){ 
        selectedOption = (selectedOption + 1) % optionsCount;

        if (currentFullscreen == 1 && selectedOption == 0) {
            selectedOption = 1; 
        }
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedOption = (selectedOption - 1 < 0) ? optionsCount - 1 : selectedOption - 1;
        
        if (currentFullscreen == 1 && selectedOption == 0) {
            selectedOption = optionsCount - 1; 
        }
    }
    if(selectedOption == 0){
        if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){
            currentResolution = (currentResolution + 1) % resolutionValues.size();
        }
        if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
            currentResolution = (currentResolution - 1 < 0) ? resolutionValues.size() - 1 : currentResolution - 1;
        }
        if(IsKeyPressed(KEY_ENTER)){
            applyResolution();
        }
  
    }
    else if(selectedOption == 1){
        if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
            currentFullscreen = (currentFullscreen == 0) ? 1: 0;
        }

        if(IsKeyPressed(KEY_ENTER)){
           applyFullscreen();
        }
        
    }
    else if(selectedOption == 2){
        if(IsKeyPressed(KEY_ENTER)){
            return AppState::MAIN_MENU;
        }
    }

    BeginTextureMode(lightMask);
        ClearBackground((Color){ 30, 30, 40, 255 }); 
        DrawCircleGradient(Vector2{VIRTUAL_WIDTH + 175, VIRTUAL_HEIGHT}, 1500, WHITE, (Color){ 30, 30, 40, 255 });
        //DrawCircleGradient(Vector2{VIRTUAL_WIDTH/2 + 175, VIRTUAL_HEIGHT/2}, 300, WHITE, (Color){ 30, 30, 40, 255 });
    EndTextureMode();

    return AppState::SETTINGS; 
}

void SettingsScreen::applyResolution(){
    Vector2 targetRes = resolutionValues[currentResolution];
    SetWindowSize((int)targetRes.x, (int)targetRes.y);
}

void SettingsScreen::applyFullscreen() {
    bool isCurrentlyFullscreen = IsWindowFullscreen();
    bool wantFullscreen = (currentFullscreen == 1);

    if (isCurrentlyFullscreen != wantFullscreen) {
        ToggleFullscreen();
    }
}

void SettingsScreen::draw() {

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


    //UI ELEMENTS
    //UI ELEMENTS

    DrawText("SETTINGS", 100, VIRTUAL_HEIGHT/ 2 - 300, 100, WHITE);

    int startX = 100;
    int startY = 200;
    int spacing = 120;

    //TODO BETTER PLACEMENT
    Color c1;
    if (currentFullscreen == 1) {
        c1 = DARKGRAY;
    } else {
        c1 = (selectedOption == 0) ? YELLOW : WHITE;
    }

    std::string resText = std::to_string((int)resolutionValues[currentResolution].x) + "x" + std::to_string((int)resolutionValues[currentResolution].y);
    
    DrawText("resolution", startX, startY, 30, c1);
    DrawText("<", startX, startY + 40, 40, c1);
    DrawText(resText.c_str(), startX + 30, startY + 40, 40, c1);
    DrawText(">", startX + MeasureText(resText.c_str(), 40) + 50, startY + 40, 40, c1);


    Color c2 = (selectedOption == 1) ? YELLOW : WHITE;
    std::string fsText = (currentFullscreen == 1) ? "YEAH" : "NAH"; 
    
    DrawText("fullscreen", startX, startY + spacing, 30, c2);
    DrawText("<", startX, startY + spacing + 40, 40, c2);
    DrawText(fsText.c_str(), startX + 30, startY + spacing + 40, 40, c2);
    DrawText(">", startX + MeasureText(fsText.c_str(), 40) + 50, startY + spacing + 40, 40, c2);

    Color c3 = (selectedOption == 2) ? BLACK : WHITE;
    Color b3 = (selectedOption == 2) ? WHITE : DARKGRAY;
    Rectangle exit = { 100, (float)startY + spacing * 2, 300, 75 };
    
    DrawRectangleRec(exit, b3);
    DrawText("Back to menu", startX +20, exit.y + 20, 30, c3);

    DrawText("[Enter] - Apply", VIRTUAL_WIDTH - MeasureText("[Enter] - Apply",20) - 5, VIRTUAL_HEIGHT - 20, 20, WHITE);

}