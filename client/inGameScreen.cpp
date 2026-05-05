#include "inGameScreen.h"

InGameScreen::InGameScreen() {
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
    float speed = myPlayer.getSpeed();
    float stamina = myPlayer.getStamina();

    if (IsKeyDown(KEY_LEFT_SHIFT) && stamina > 0) {
        speed *= 1.5f; 
        myPlayer.setStamina(stamina - 1);
    } else if(stamina < 100) {
        myPlayer.setStamina(stamina + 0.2f); 
    }

    float pX = myPlayer.getX();
    float pY = myPlayer.getY();

    if(IsKeyDown(KEY_W)) {
        pY -= speed;
    }
    if(IsKeyDown(KEY_S)) {
        pY += speed;
    }
    if(IsKeyDown(KEY_A)) {
        pX -= speed;
    }
    if(IsKeyDown(KEY_D)) {
        pX += speed;
    }

    myPlayer.setX(pX);
    myPlayer.setY(pY);

    camera.target = (Vector2){myPlayer.getX() + 10, myPlayer.getY()+10};

    if(IsKeyPressed(KEY_ESCAPE)){
        return AppState::MAIN_MENU;
    }


    return AppState::IN_GAME; 
}

void InGameScreen::draw(){

    BeginMode2D(camera);
        DrawGrid(100, 10);
        DrawRectangle(myPlayer.getX(), myPlayer.getY(), 20, 30, RED); // CHANFGE TO CONSTANTS WIDTH & HEIGHT
        DrawRectangle(200,200,20,30, BLUE); // Dummy Seeker
    EndMode2D();
        
        
    DrawText("Stamina: ", 10, 10, 20,WHITE);
    DrawRectangle(10,40, myPlayer.getStamina() * 2, 20, GREEN);
        
    DrawRectangleLines(10,40, 200, 20, WHITE);

    
}