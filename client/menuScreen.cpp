#include "menuScreen.h"

AppState MenuScreen::update(){
    if (IsKeyPressed(KEY_DOWN)){
        selectedOption = (selectedOption + 1) % optionsCount;
    }
    if(IsKeyPressed(KEY_UP)){
        selectedOption = (selectedOption - 1 < 0) ? optionsCount - 1 : selectedOption - 1;
    }

    if (IsKeyPressed(KEY_ENTER)){
        switch (selectedOption){
            case 0:
                return AppState::CONNECTING;
                break;
            case 1:   
                return AppState::EXIT;      
                break;
        }
    }

    return AppState::MAIN_MENU; // Stay on the main menu if no option is selected
}

void MenuScreen::draw(){
    Color startColor = (selectedOption == 0) ? DARKGRAY : LIGHTGRAY; 
    Color exitColor = (selectedOption == 1) ? DARKGRAY : LIGHTGRAY; 

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    DrawText("SNEAK&PEAK", screenWidth / 2 - 200, screenHeight / 2 - 200, 50, WHITE); // TODO - center text properly
    DrawRectangle(screenWidth / 2 - 100, 300, 200, 50, startColor); 
    DrawText("Start Game", screenWidth / 2 - 90, 310, 20, BLACK);   // TODO -  Buttons too...
    DrawRectangle(screenWidth / 2 - 100, 400, 200, 50, exitColor);
    DrawText("Exit", screenWidth / 2 - 80, 410, 20, BLACK);

}