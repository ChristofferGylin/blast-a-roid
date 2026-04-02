#include "raylib.h"
#include "gameloop.h"
#include <time.h>

int main(){

    int windowWidth = 800;
    int windowHeight = 600;

    InitWindow(windowWidth, windowHeight, "basic window");
    InitAudioDevice();
    SetTargetFPS(144);
    SetRandomSeed(time(NULL));

    int level = 0;

    gameLoop(level);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}