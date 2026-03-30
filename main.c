#include "raylib.h"

int main(){

    int windowWidth = 800;
    int windowHeight = 600;

    InitWindow(windowWidth, windowHeight, "basic window");
    InitAudioDevice();
    SetTargetFPS(144);

    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}