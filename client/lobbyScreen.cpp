#include "lobbyScreen.h"
#include "../shared/contstants.h"
#include <iostream>

LobbyScreen :: LobbyScreen(){
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

LobbyScreen :: ~LobbyScreen(){
    bgMap.unload();
    UnloadShader(visionShader);
    UnloadRenderTexture(lightMask); 
}

AppState LobbyScreen :: update(){
    frameCounter ++;

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) active = (active + 1) % 4;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) active = (active - 1 < 0) ? 3 : active - 1;

    if (active == 0 || active == 1){
        int key = GetCharPressed();
        while(key > 0){
            //ip
            if(active == 0 && (key >= 48 && key <= 57) || key == 46 ){ // Data vaidation
                if(ipInput.length() < 15) ipInput += (char)key;
            }
            //port
            else if(active == 1 && key >= 48 && key <= 57){ // Data validation
                if (portInput.length() < 4) portInput += (char)key;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)){
            if(active == 0 && ipInput.length() > 0) ipInput.pop_back();
            if(active == 1 && portInput.length() > 0) portInput.pop_back();
        }

    }


    if (IsKeyPressed(KEY_ENTER)){
        if(active == 2) return AppState::CONNECTING;
        if(active == 3) return AppState::MAIN_MENU;
    }



    BeginTextureMode(lightMask);
        ClearBackground((Color){ 30, 30, 40, 255 }); 

        DrawCircleGradient(Vector2{VIRTUAL_WIDTH + 175, VIRTUAL_HEIGHT}, 1500, WHITE, (Color){ 30, 30, 40, 255 });
        //DrawCircleGradient(Vector2{VIRTUAL_WIDTH/2 + 175, VIRTUAL_HEIGHT/2}, 300, WHITE, (Color){ 30, 30, 40, 255 });
    EndTextureMode();

    return AppState::LOBBY;
}

void LobbyScreen :: draw(){

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

    int startX = 100;
    int startY = 200;
    int spacing = 120;

    DrawText("JOIN SERVER", 100, VIRTUAL_HEIGHT/ 2 - 300, 100, WHITE);

    Color c1 = (active == 0) ? YELLOW : WHITE;

    Color c2 = (active == 1) ? YELLOW : WHITE;

    Color c3 = (active == 2) ? BLACK : WHITE;
    Color b3 = (active == 2) ? WHITE : DARKGRAY;

    Color c4 = (active == 3) ? BLACK : WHITE;
    Color b4 = (active == 3) ? WHITE : DARKGRAY;

    DrawText("IP Address:", startX, startY, 30, c1);
    DrawRectangleLines(startX, startY + 40, 300,50,c1);
    DrawText(ipInput.c_str(), startX + 10, startY + 50, 30, c1);
    if (active == 0 && (frameCounter / 20) % 2 == 0) DrawText(" |", startX + 10 + MeasureText(ipInput.c_str(), 30), startY + 50, 30, c1);

    DrawText("Port:", startX, startY+spacing, 30, c2);
    DrawRectangleLines(startX, startY + spacing + 40, 300,50, c2);
    DrawText(portInput.c_str(), startX + 10, startY + spacing + 50, 30, c2);
    if (active == 1 && (frameCounter / 20) % 2 == 0) DrawText(" |", startX + 10 + MeasureText(portInput.c_str(), 30), startY + spacing + 50, 30, c2);

    Rectangle connect = {(float)startX, (float)startY + 2*spacing, 300,75};
    DrawRectangleRec(connect, b3);
    DrawText("Connect", startX + 10, connect.y + 20, 30, c3);

    Rectangle back = {(float)startX, (float)startY + 3*spacing, 300,75};
    DrawRectangleRec(back, b4);
    DrawText("Back to Menu", startX + 10, back.y + 20, 30, c4);
}
