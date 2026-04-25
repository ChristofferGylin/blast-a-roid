#include "raylib.h"
#include <time.h>
#include "constants.h"
#include "mainMenu.h"
#include "gameContext.h"

GameContext ctx;

int main(){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blast-A-Roid");
    InitAudioDevice();
    SetTargetFPS(144);
    SetRandomSeed(time(NULL));
    SetExitKey(KEY_NULL);
    initGameContext(&ctx);
    
    mainMenu(&ctx);
    
    unloadAssets(&ctx);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

