#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "raylib.h"
#include "constants.h"
#include "config.h"
#include "mainMenu.h"
#include "gameContext.h"

GameContext ctx;

int main(int argc, char* argv[]){

    bool isDebugActive = false;

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-d") == 0) {
                isDebugActive = true;
                break;
            }
        }
    }

    initConfig(&ctx);

    if (ctx.options.video.isVSyncEnabled) {
        SetConfigFlags(FLAG_VSYNC_HINT);
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blast-A-Roid");
    InitAudioDevice();

    if (!ctx.options.video.isVSyncEnabled) {
        setUserRefreshRate();
    }

    SetRandomSeed(time(NULL));
    srand((unsigned)time(NULL));
    SetExitKey(KEY_NULL);
    initGameContext(&ctx, isDebugActive);
    
    mainMenu(&ctx);
    
    unloadAssets(&ctx);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

