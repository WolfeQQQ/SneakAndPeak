#include "inGameScreen.h"
#include <cmath>

InGameScreen::InGameScreen(Network* networkClient, int playerId) {
    network = networkClient;

    camera = { 0 };
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.5f;

    this->playerId = playerId;

    tilemap.load("assets/map_temp.csv", "assets/tilemap.png", 36); // change to constants later
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

    for (int i = 0; i < 4; i++){
        if(players[i].getId() == this ->playerId){
            myPlayer = players[i];
            break;
        }
    }

    camera.target = (Vector2){std::round(myPlayer.getX()) + 10, std::round(myPlayer.getY())+10};

    // if(IsKeyPressed(KEY_ESCAPE)){
    //     return AppState::MAIN_MENU;
    // }

    return AppState::IN_GAME; 
}

void InGameScreen::draw(){

    BeginMode2D(camera);
        tilemap.draw();
        for(int i = 0; i<4; i++){
            if(players[i].getIsConnected()){
                Color playerColor = players[i].getIsSeeker() ? RED : BLUE;
                DrawRectangle(std::round(players[i].getX()), std::round(players[i].getY()), 20, 30, playerColor);

                DrawText(std::to_string(players[i].getId()).c_str(), std::round(players[i].getX()), std::round(players[i].getY()) - 20, 10, WHITE);
            }
        }
    EndMode2D();
        
        
    DrawText("Stamina: ", 10, 10, 20,WHITE);
    DrawRectangle(10,40, myPlayer.getStamina() * 2, 20, GREEN);
    DrawRectangleLines(10,40, 200, 20, WHITE);

    
}