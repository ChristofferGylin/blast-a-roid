#include "options.h"

void resetOptionsToDefault(Options* options) {
    options->video.showFps = SHOW_FPS_DEFAULT_VALUE;
    options->video.isVSyncEnabled = IS_V_SYNC_ENABLED_DEFAULT_VALUE;
}