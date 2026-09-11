#include "options.h"
#include "raylib.h"

void setUserRefreshRate() {

    int refreshRate = GetMonitorRefreshRate(GetCurrentMonitor());

    if (refreshRate <= 0) {
        refreshRate = 60;
    }

    SetTargetFPS(refreshRate);
    
}

void resetControlsToDefault(ControlsOptions* controls) {
    controls->isControlsSetByUser = false;
    controls->keys.fire = KEY_BIND_FIRE_DEFAULT_VALUE;
    controls->keys.left = KEY_BIND_LEFT_DEFAULT_VALUE;
    controls->keys.right = KEY_BIND_RIGHT_DEFAULT_VALUE;
    controls->keys.shield = KEY_BIND_SHIELD_DEFAULT_VALUE;
    controls->keys.thrust = KEY_BIND_THRUST_DEFAULT_VALUE;
}

void resetOptionsToDefault(Options* options) {
    options->video.fullscreen = FULLSCREEN_DEFAULT_VALUE;
    options->video.isMonitorSetByUser = IS_MONITOR_SET_BY_USER_DEFAULT_VALUE;
    options->video.selectecMonitor = SELECTED_MONITOR_DEFAULT_VALUE;
    options->video.showFps = SHOW_FPS_DEFAULT_VALUE;
    options->video.vSync = IS_V_SYNC_ENABLED_DEFAULT_VALUE;
}