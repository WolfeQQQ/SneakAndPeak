#include "gameApp.h"
#include "menuScreen.h"
#include "inGameScreen.h"
#include "../shared/contstants.h"
#include "gameOverScreen.h"
// #include more_screens
#include "raylib.h"

/*
Main application manager, responsible for initializing the window and managing the main game loop.
Handles the current state of the application and transitions beetween different screens.
*/

// Constructor, initializes the application window and sets the initial state
GameApp::GameApp() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sneak&Peak"); // Initialize the window with a title
    SetExitKey(0); // Disable the default ESC key behavior

    changeState(AppState::MAIN_MENU); // Game Starts with the main menu
}

// Destructor, cleans up resources
GameApp::~GameApp() {
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
    BeginDrawing();
    ClearBackground(BLACK);
    if(currentScreen){
        currentScreen->draw();
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
        
    }
}

