#include "gameOverScreen.h"
#include <cmath>
#include <string>

GameOverScreen::GameOverScreen(Network * clientNetwork, const GameStatePacket& finalPacket){
    network = clientNetwork;
    lastPacket = finalPacket;
    backToLobbyPressed = false;
    selectedOption = 0;
}

GameOverScreen::~GameOverScreen() {}

AppState GameOverScreen::update(){
    if (IsKeyPressed(KEY_S)) {
        selectedOption++;
        if (selectedOption >= optionsCount) {
            selectedOption = 0; 
        }
    }
    if (IsKeyPressed(KEY_W)) {
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
            return AppState::EXIT;
        }
    }
    GameStatePacket packet;
    if (network != nullptr && network->ReceiveState(packet)) {
        if (packet.stage == GameState::LOBBY) {
            return AppState::LOBBY;
        }
    }

    return AppState::GAME_OVER;
}

void GameOverScreen::draw() {
    ClearBackground((Color){ 10, 10, 15, 255 });

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
    DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, lobbyColor);


    DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, lobbyColor);

    std::string lobbyText = backToLobbyPressed ? "WAITING..." : "BACK TO LOBBY";
    int lobbyTextWidth = MeasureText(lobbyText.c_str(), 20);
    DrawText(lobbyText.c_str(), buttonX + (buttonWidth / 2 - lobbyTextWidth / 2), buttonY + (buttonHeight / 2 - 10), 20, WHITE);


    int buttonExitY = buttonY + 70;


    Color exitColor = (selectedOption == 1) ? WHITE : DARKGRAY;

    DrawRectangle(buttonX, buttonExitY, buttonWidth, buttonHeight, exitColor);

    std::string exitText = "EXIT GAME";
    int exitTextWidth = MeasureText(exitText.c_str(), 20);
    DrawText(exitText.c_str(), buttonX + (buttonWidth / 2 - exitTextWidth / 2), buttonExitY + (buttonHeight / 2 - 10), 20, WHITE);
}
