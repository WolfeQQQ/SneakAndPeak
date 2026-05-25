#include "menuScreen.h"

// Whole menu update logic. Handles navigation and selection of options. Returns the next state to transition to.
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

// Renders the Main menu
void MenuScreen::draw(){

    // Colors for the currently selected and non-selected options
    Color startColor = (selectedOption == 0) ? LIGHTGRAY : DARKGRAY; 
    Color exitColor = (selectedOption == 1) ? LIGHTGRAY : DARKGRAY; 

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    DrawText("SNEAK&PEAK", 100, screenHeight / 2 - 200, 100, WHITE); // TODO - center text properly
    
    DrawRectangle(100, 300, 200, 50, startColor); 
    DrawText("Start Game", 100, 310, 20, BLACK);   // TODO -  Buttons too...
    
    DrawRectangle(100, 400, 200, 50, exitColor);
    DrawText("Exit", 100, 410, 20, BLACK);

    //TODO COORDINATES AND SIZE ADJUSTMENTS
}