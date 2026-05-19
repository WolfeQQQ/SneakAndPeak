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

    playerIdleTexture = LoadTexture("assets/player/blue/idle.png");
    playerWalkTexture = LoadTexture("assets/player/blue/walk.png");
    playerDeathTexture = LoadTexture("assets/player/blue/death.png");

    seekerIdleTexture = LoadTexture("assets/player/red/idle.png");
    seekerWalkTexture = LoadTexture("assets/player/red/walk.png");
    seekerDeathTexture = LoadTexture("assets/player/red/death.png");

    tilemap.load("assets/map_temp.csv", "assets/tilemap.png", 36); // change to constants later
}

InGameScreen::~InGameScreen() {
    UnloadTexture(playerIdleTexture);
    UnloadTexture(playerWalkTexture);
    UnloadTexture(playerDeathTexture);
    UnloadTexture(seekerIdleTexture);
    UnloadTexture(seekerWalkTexture);
    UnloadTexture(seekerDeathTexture);
    tilemap.unload();
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

    GameStatePacket packet;

    if(network ->ReceiveState(packet)){
        for(int i = 0; i < 4; i++){
            players[i] = packet.players[i];
        }
    }
    gameTimer = packet.timer;

    for(int i = 0; i< 4; i++){
        if(!players[i].getIsConnected()) continue;

        if(players[i].getId() == this ->playerId){
            myPlayer = players[i];
        }

        PlayerAnimation& anim = playerAnim[i];
        
        player::PlayerState currentState = players[i].getPlayerState();

        if(anim.lastState != currentState){
            anim.frame = 0;
            anim.frameTimer = 0.0f;
            anim.lastState = currentState;
        }

        int maxFrames = 2;
        if(currentState == player::PlayerState::DEATH) maxFrames = 3;
        if(currentState == player::PlayerState::WALK) maxFrames = 4;

        anim.frameTimer += GetFrameTime();
        if(anim.frameTimer >= 0.2f){
            anim.frameTimer = 0.0f;

            if(currentState == player::PlayerState::DEATH){
                if(anim.frame < maxFrames - 1){
                    anim.frame++;
                }
            }
            else{
                anim.frame = (anim.frame + 1) % maxFrames;
            }

        }

    }

    camera.target = (Vector2){std::round(myPlayer.getX()) + 10, std::round(myPlayer.getY())+15};

    // if(IsKeyPressed(KEY_ESCAPE)){
    //     return AppState::MAIN_MENU;
    // }

    return AppState::IN_GAME; 
}

void InGameScreen::draw(){

    BeginMode2D(camera);

        tilemap.draw();

        for(int i = 0; i<4; i++){

            if(!players[i].getIsConnected()) continue;

            PlayerAnimation& anim = playerAnim[i];
            player::PlayerState currentState = players[i].getPlayerState();
        
            player::Direction direction = players[i].getDirection();
           

            Texture2D currentTexture;
            switch (currentState)
            {
            case player::PlayerState::IDLE:
                if(players[i].getIsSeeker()) currentTexture = seekerIdleTexture;
                else currentTexture = playerIdleTexture;
    
                break;
            case player::PlayerState::WALK:
                if(players[i].getIsSeeker()) currentTexture = seekerWalkTexture;
                else currentTexture = playerWalkTexture;
                break;
            case player::PlayerState::DEATH:
                if(players[i].getIsSeeker()) currentTexture = seekerDeathTexture;
                else currentTexture = playerDeathTexture;
                break;
            default:
                currentTexture = playerIdleTexture;
                break;
            }

            int row;
            if(direction == player::Direction::DOWN) row = 0;
            else if(direction == player::Direction::LEFT || direction == player::Direction::RIGHT) row = 1;
            else if(direction == player::Direction::UP) row = 2;

            float frameWidth = 32.0f;
            if(direction == player::Direction::LEFT){
                frameWidth = -32.0f;
            }

            Rectangle source = {(float)(anim.frame * 32), (float)(row * 32), frameWidth, 32.0f};

            float scale = 2.0f;
            float drawSize = 32.0f * scale;
            float dX = players[i].getX() - (drawSize / 2) + 10;
            float dY = players[i].getY() - (drawSize / 2) + 15;
            Rectangle dest = {dX, dY, drawSize, drawSize};
            DrawTexturePro(currentTexture, source, dest, (Vector2){0,0}, 0.0f, WHITE);

            if(players[i].getIsConnected()){
                // Color playerColor = players[i].getIsSeeker() ? RED : BLUE;
                // DrawRectangle(std::round(players[i].getX()), std::round(players[i].getY()), 20, 30, playerColor);

                DrawText(std::to_string(players[i].getId()).c_str(), std::round(players[i].getX()), std::round(players[i].getY()) - 20, 10, WHITE);
            }


        }



    EndMode2D();
        
        
    DrawText("Stamina: ", 10, 10, 20,WHITE);
    DrawRectangle(10,40, myPlayer.getStamina() * 2, 20, GREEN);
    DrawRectangleLines(10,40, 200, 20, WHITE);
    DrawText(std::to_string(gameTimer).c_str(),GetScreenWidth()/2 - 25, 30, 20, WHITE);
   

    
}