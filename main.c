#include "raylib.h"
#include "gameloop.h"
#include <time.h>
#include "constants.h"

int main(){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blast-A-Roid");
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