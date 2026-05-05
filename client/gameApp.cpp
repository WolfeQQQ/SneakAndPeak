#include "gameApp.h"
#include "menuScreen.h"
#include "inGameScreen.h"

// another screens

#include "raylib.h"

/*
Main application manager, responsible for initializing the window and managing the main game loop.
Handles the current state of the application and transitions beetween different screens.
*/

GameApp::GameApp() {
    InitWindow(1280,720, "Sneak&Peak"); // Initialize the window with a title
    SetTargetFPS(60);
    SetExitKey(0);
    changeState(AppState::MAIN_MENU); // Start with the main menu
}

GameApp::~GameApp() {
    // Destructor code 
    CloseWindow(); 
}

void GameApp::run(){
    while(!WindowShouldClose() && currentState != AppState::EXIT){
        update();
        draw();
    }
}

void GameApp::update(){
    if(currentScreen){
        AppState newState = currentScreen->update();
        if (newState != currentState){
            changeState(newState);
        }
    }

}

void GameApp::draw(){
    BeginDrawing();
    ClearBackground(BLACK);
    if(currentScreen){
        currentScreen->draw();
    }
    EndDrawing();
}

void GameApp::changeState(AppState newState){
    currentState = newState;
    switch(newState){
        case AppState::MAIN_MENU:
            currentScreen = std::make_unique<MenuScreen>();
            break;
        case AppState::CONNECTING:
            // currentScreen = std::make_unique<ConnectingScreen>();
            break;
        case AppState::LOBBY:
            // currentScreen = std::make_unique<LobbyScreen>();
            break;
        case AppState::IN_GAME:
            currentScreen = std::make_unique<InGameScreen>();
            break;
        case AppState::DISCONNECTED:
            // currentScreen = std::make_unique<DisconnectedScreen>();
            break;
        case AppState::GAME_OVER:
            // currentScreen = std::make_unique<GameOverScreen>();
            break;
        
    }
}

