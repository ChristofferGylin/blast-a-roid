#include "raylib.h"
#include "gameloop.h"
#include <time.h>

int main(){

    int windowWidth = 800;
    int windowHeight = 600;

    InitWindow(windowWidth, windowHeight, "Blast-A-Roid");
    InitAudioDevice();
    SetTargetFPS(144);
    SetRandomSeed(time(NULL));

    Player player = {
        2, 1, 0, 0, 2, 50, {0}
    };

    gameLoop(&player);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}