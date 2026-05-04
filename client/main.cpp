#include <iostream>
#include "raylib.h"


int main(){
    InitWindow(1280,720, "Sneak&Peak");
    SetTargetFPS(60);

    int px = GetScreenWidth()/2;
    int py = GetScreenHeight()/2;

    while (!WindowShouldClose()){

        
        //draw loop
       switch (GetKeyPressed())
        {
            case KEY_A:
                px -= 10;
                break;
            case KEY_D:
                px += 10;
                break;
            case KEY_W:
                py -= 10;
                break;
            case KEY_S:
                py += 10;
                break;
            default:
                break;


        }


        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(px,py,10,10,RED);
        EndDrawing();


        //draw loop//
    }


    return 0;
}