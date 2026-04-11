#include "raylib.h"
#include <time.h>
#include "constants.h"
#include "mainMenu.h"

int main(){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blast-A-Roid");
    InitAudioDevice();
    SetTargetFPS(144);
    SetRandomSeed(time(NULL));
    
    mainMenu();
    
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

