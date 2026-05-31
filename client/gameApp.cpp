#include "../shared/contstants.h"
#include "gameApp.h"
#include "menuScreen.h"
#include "inGameScreen.h"
#include "gameOverScreen.h"
#include "settingsScreen.h"
// #include more_screens
#include <cmath>

/*
Main application manager, responsible for initializing the window and managing the main game loop.
Handles the current state of the application and transitions beetween different screens.
*/

// Constructor, initializes the application window and sets the initial state
GameApp::GameApp() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);


    InitWindow(START_SCREEN_WIDTH, START_SCREEN_HEIGHT, "Sneak&Peak"); // Initialize the window with a title
    SetExitKey(0); // Disable the default ESC key behavior


    virtualCanvas = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    SetTextureFilter(virtualCanvas.texture, TEXTURE_FILTER_POINT);

    changeState(AppState::MAIN_MENU); // Game Starts with the main menu
}

// Destructor, cleans up resources
GameApp::~GameApp() {

    UnloadRenderTexture(virtualCanvas);
    CloseWindow(); // :O
}

// Main game loop, handles updating and drawing
void GameApp::run(){
    while(!WindowShouldClose() && currentState != AppState::EXIT){  // Loop until the window should close or the state is EXIT
        update();
        draw();
    }
}

// Method that handles the application state changes and calls the update method of the current screen
void GameApp::update(){
    if(currentScreen){
        AppState newState = currentScreen->update();
        if (newState != currentState){
            changeState(newState);
        }
    }

}

// Simply calls the draw method of the current screen
void GameApp::draw(){

    BeginTextureMode(virtualCanvas);
        ClearBackground(BLACK);
        if(currentScreen && !isFading){
            currentScreen -> draw();
        }
    EndTextureMode();

    BeginDrawing();
        ClearBackground(BLACK);
        
        float scale = fminf((float)GetScreenWidth()/ VIRTUAL_WIDTH, (float) GetScreenHeight()/VIRTUAL_HEIGHT);
        Rectangle sourceRec = {0.0f,0.0f,   (float)virtualCanvas.texture.width, -(float)virtualCanvas.texture.height};

        Rectangle destRec = {
            ((float)GetScreenWidth() - ((float)VIRTUAL_WIDTH * scale)) * 0.5f,
            ((float)GetScreenHeight() - ((float)VIRTUAL_HEIGHT * scale)) * 0.5f,
            (float)VIRTUAL_WIDTH * scale,
            (float)VIRTUAL_HEIGHT * scale
        };

        DrawTexturePro(virtualCanvas.texture, sourceRec, destRec, (Vector2){ 0, 0 }, 0.0f, WHITE);

        if (fadeAlpha > 0.0f) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fadeAlpha));
        }

    EndDrawing();
}

// Game's state manager, Destroy the current screen and create a new one based on the state
void GameApp::changeState(AppState newState){
    currentState = newState;
    switch(newState){
        case AppState::MAIN_MENU:
            currentScreen = std::make_unique<MenuScreen>();
            break;
        case AppState::CONNECTING:  // State that attempts to conect to the server, if successful, starts the game.

            //currentScreen = std::make_unique<ConnectingScreen>();

            if(networkClient.ConnectToServer(SERVER_IP, SERVER_PORT, &playerId)==true)
            { //MOVE TO CONNECTING SCREEN
                changeState(AppState::IN_GAME);
            } 
            else {
                changeState(AppState::MAIN_MENU);
                //changeState(AppState::DISCONNECTED);
            }
            break;

        case AppState::LOBBY:
            // currentScreen = std::make_unique<LobbyScreen>();
            break;
        case AppState::IN_GAME:
            currentScreen = std::make_unique<InGameScreen>(&networkClient, playerId);
            break;
        case AppState::DISCONNECTED:
            // currentScreen = std::make_unique<DisconnectedScreen>();
            break;
        case AppState::GAME_OVER:
            if (currentScreen) {
                auto inGame = dynamic_cast<InGameScreen*>(currentScreen.get());
                if (inGame) {
                    lastPacket = inGame->getLastPacket();
                }
            }
             currentScreen = std::make_unique<GameOverScreen>(&networkClient, lastPacket);
            break;
        case AppState::SETTINGS:
            currentScreen = std::make_unique<SettingsScreen>();
            break;
        
    }
}

