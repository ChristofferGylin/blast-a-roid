#include "raylib.h"
#include "gameloop.h"

int main(){

    int windowWidth = 800;
    int windowHeight = 600;

    InitWindow(windowWidth, windowHeight, "basic window");
    InitAudioDevice();
    SetTargetFPS(144);

    int level = 0;

    gameLoop(level);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}