#include "tutorialScreen.h"
#include <cmath>
#include <string>

TutorialScreen::TutorialScreen(){
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

    texW = LoadTexture("assets/ui/W.png");
    texA = LoadTexture("assets/ui/A.png");
    texS = LoadTexture("assets/ui/S.png");
    texD = LoadTexture("assets/ui/D.png");
    texArrUp = LoadTexture("assets/ui/ARROWUP.png");
    texArrDown = LoadTexture("assets/ui/ARROWDOWN.png");
    texArrLeft = LoadTexture("assets/ui/ARROWLEFT.png");
    texArrRight = LoadTexture("assets/ui/ARROWRIGHT.png");
    texShift = LoadTexture("assets/ui/SHIFT.png");

}

TutorialScreen::~TutorialScreen(){
    bgMap.unload();
    UnloadShader(visionShader);
    UnloadRenderTexture(lightMask); 

    UnloadTexture(texW);
    UnloadTexture(texA);
    UnloadTexture(texS);
    UnloadTexture(texD);
    UnloadTexture(texArrUp);
    UnloadTexture(texArrDown);
    UnloadTexture(texArrLeft);
    UnloadTexture(texArrRight);
    UnloadTexture(texShift);
}

AppState TutorialScreen::update(){
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)){
        return AppState::MAIN_MENU;
    }
    return AppState::TUTORIAL;
}
void TutorialScreen::draw(){
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

    DrawText("HOW TO PLAY", VIRTUAL_WIDTH/2 - MeasureText("HOW TO PLAY", 70)/2, 40, 70, WHITE);

    float keyScale = 2.5f; 
    float keySize = texW.width * keyScale; 
    float keySpacing = 8.0f;
    float topY = 140.0f;

    float leftBaseX = VIRTUAL_WIDTH/2 - 200;
    DrawTextureEx(texW, {leftBaseX + keySize + keySpacing, topY}, 0.0f, keyScale, WHITE);
    DrawTextureEx(texA, {leftBaseX, topY + keySize + keySpacing}, 0.0f, keyScale, WHITE);
    DrawTextureEx(texS, {leftBaseX + keySize + keySpacing, topY + keySize + keySpacing}, 0.0f, keyScale, WHITE);
    DrawTextureEx(texD, {leftBaseX + (keySize + keySpacing)*2, topY + keySize + keySpacing}, 0.0f, keyScale, WHITE);

    float rightBaseX = VIRTUAL_WIDTH/2 + 34;
    DrawTextureEx(texArrUp, {rightBaseX + keySize + keySpacing, topY}, 0.0f, keyScale, WHITE);
    DrawTextureEx(texArrLeft, {rightBaseX, topY + keySize + keySpacing}, 0.0f, keyScale, WHITE);
    DrawTextureEx(texArrDown, {rightBaseX + keySize + keySpacing, topY + keySize + keySpacing}, 0.0f, keyScale, WHITE);
    DrawTextureEx(texArrRight, {rightBaseX + (keySize + keySpacing)*2, topY + keySize + keySpacing}, 0.0f, keyScale, WHITE);

    DrawLineEx({VIRTUAL_WIDTH/2 - 15, topY + keySize*2 + 10}, {VIRTUAL_WIDTH/2 + 15, topY - 10}, 3, LIGHTGRAY);
    DrawText("WALK", VIRTUAL_WIDTH/2 - MeasureText("WALK", 30)/2, topY + keySize*2 + 30, 30, LIGHTGRAY);

    // Separating line
    float sepY = VIRTUAL_HEIGHT/2 - 20; 
    DrawLineEx({100, sepY}, {VIRTUAL_WIDTH - 100, sepY}, 2, LIGHTGRAY);

    float shiftScale = 3.0f;
    float shiftW = texShift.width * shiftScale;

    float hiderCenterX = VIRTUAL_WIDTH * 0.25f;
    DrawText("THE HIDER", hiderCenterX - MeasureText("THE HIDER", 50)/2, sepY + 30, 50, SKYBLUE);
    DrawTextureEx(texShift, {hiderCenterX - shiftW/2, sepY + 90}, 0.0f, shiftScale, WHITE);
    DrawText("SPECIAL ABILITY: RADAR", hiderCenterX - MeasureText("SPECIAL ABILITY: RADAR", 25)/2, sepY + 180, 25, WHITE);
    DrawText("Hold to reveal the Seeker's direction.", hiderCenterX - MeasureText("Hold to reveal the Seeker's direction.", 20)/2, sepY + 220, 20, LIGHTGRAY);
    DrawText("Has a 10-second cooldown after use.", hiderCenterX - MeasureText("Has a 10-second cooldown after use.", 20)/2, sepY + 250, 20, LIGHTGRAY);

    float seekerCenterX = VIRTUAL_WIDTH * 0.75f;
    DrawText("THE SEEKER", seekerCenterX - MeasureText("THE SEEKER", 50)/2, sepY + 30, 50, RED);
    DrawTextureEx(texShift, {seekerCenterX - shiftW/2, sepY + 90}, 0.0f, shiftScale, WHITE);
    DrawText("SPECIAL ABILITY: SPRINT", seekerCenterX - MeasureText("SPECIAL ABILITY: SPRINT", 25)/2, sepY + 180, 25, WHITE);
    DrawText("Hold to run much faster.", seekerCenterX - MeasureText("Hold to run much faster.", 20)/2, sepY + 220, 20, LIGHTGRAY);
    DrawText("Watch your stamina bar from overheating!", seekerCenterX - MeasureText("Watch your stamina bar from overheating!", 20)/2, sepY + 250, 20, LIGHTGRAY);

    DrawText("Press [ESC]/[ENTER] to return", VIRTUAL_WIDTH/2 - MeasureText("Press [ESC]/[ENTER] to return", 25)/2, VIRTUAL_HEIGHT - 40, 25, DARKGRAY);
}