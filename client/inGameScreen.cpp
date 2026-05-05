#include "inGameScreen.h"

InGameScreen::InGameScreen(Network* networkClient) {
    network = networkClient;

    myPlayer.setX(100);
    myPlayer.setY(100);
    myPlayer.setSpeed(4);
    myPlayer.setStamina(100);

    camera = { 0 };
    camera.target = (Vector2){myPlayer.getX()+10, myPlayer.getY()+10};
    camera.offset = (Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.5f;
}


AppState InGameScreen::update(){

    player::ClientInput input;

    input.up = IsKeyDown(KEY_W);
    input.down = IsKeyDown(KEY_S);
    input.left = IsKeyDown(KEY_A);
    input.right = IsKeyDown(KEY_D);
    input.shift = IsKeyDown(KEY_LEFT_SHIFT);
    //input.eKey = IsKeyDown(KEY_E);
    

    network ->SendInput(input);
    network ->ReceiveState(players);


    camera.target = (Vector2){myPlayer.getX() + 10, myPlayer.getY()+10};

    // if(IsKeyPressed(KEY_ESCAPE)){
    //     return AppState::MAIN_MENU;
    // }

    return AppState::IN_GAME; 
}

void InGameScreen::draw(){

    BeginMode2D(camera);
        for(int i = 0; i<4; i++){
            if(players[i].getIsConnected()){
                Color playerColor = players[i].getIsSeeker() ? RED : BLUE;
                DrawRectangle(players[i].getX(), players[i].getY(), 20, 20, playerColor);

                //add a little number to show player index
            }
        }
    EndMode2D();
        
        
    DrawText("Stamina: ", 10, 10, 20,WHITE);
    // DrawRectangle(10,40, myPlayer.getStamina() * 2, 20, GREEN);
        
    DrawRectangleLines(10,40, 200, 20, WHITE);

    
}