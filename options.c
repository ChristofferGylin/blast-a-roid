#include "options.h"
#include "raylib.h"

void setUserRefreshRate() {

    int refreshRate = GetMonitorRefreshRate(GetCurrentMonitor());

    if (refreshRate <= 0) {
        refreshRate = 60;
    }

    SetTargetFPS(refreshRate);
    
}

void resetOptionsToDefault(Options* options) {
    options->video.isMonitorSetByUser = false;
    options->video.selectecMonitor = 0;
    options->video.showFps = SHOW_FPS_DEFAULT_VALUE;
    options->video.vSync = IS_V_SYNC_ENABLED_DEFAULT_VALUE;
}