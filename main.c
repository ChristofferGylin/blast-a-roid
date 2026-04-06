#include "raylib.h"
#include "gameloop.h"
#include <time.h>

int main(){

    int windowWidth = 1280;
    int windowHeight = 720;

    InitWindow(windowWidth, windowHeight, "Blast-A-Roid");
    InitAudioDevice();
    SetTargetFPS(144);
    SetRandomSeed(time(NULL));

    Player player = {0};
    initPlayer(&player);

    gameLoop(&player);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}