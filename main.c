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

    if (ctx.options.video.vSync) {
        SetConfigFlags(FLAG_VSYNC_HINT);
        ctx.isVsyncEnabled = true;
    } else {
        ctx.isVsyncEnabled = false;
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blast-A-Roid");
    InitAudioDevice();

    if (
        ctx.options.video.isMonitorSetByUser &&
        ctx.options.video.selectecMonitor >= 0 &&
        ctx.options.video.selectecMonitor < GetMonitorCount()
    ) {
        SetWindowMonitor(ctx.options.video.selectecMonitor);
    }

    if (!ctx.options.video.vSync) {
        setUserRefreshRate();
    }

    if (ctx.options.video.fullscreen) {
        ToggleFullscreen();
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

