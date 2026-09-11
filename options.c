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
    controls->keys.fire = KEY_BIND_FIRE_DEFAULT_VALUE;
    controls->keys.left = KEY_BIND_LEFT_DEFAULT_VALUE;
    controls->keys.right = KEY_BIND_RIGHT_DEFAULT_VALUE;
    controls->keys.shield = KEY_BIND_SHIELD_DEFAULT_VALUE;
    controls->keys.thrust = KEY_BIND_THRUST_DEFAULT_VALUE;
}

void resetOptionsToDefault(Options* options) {
    resetControlsToDefault(&options->controls);
    resetVideoOptionsToDefault(&options->video);
}

void resetVideoOptionsToDefault(VideoOptions* options) {
    options->fullscreen = FULLSCREEN_DEFAULT_VALUE;
    options->isMonitorSetByUser = IS_MONITOR_SET_BY_USER_DEFAULT_VALUE;
    options->selectecMonitor = SELECTED_MONITOR_DEFAULT_VALUE;
    options->showFps = SHOW_FPS_DEFAULT_VALUE;
    options->vSync = IS_V_SYNC_ENABLED_DEFAULT_VALUE;
}