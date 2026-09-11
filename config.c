#include <stdio.h>

#include "config.h"
#include "debug.h"
#include "gameContext.h"
#include "options.h"
#include "raylib.h"

bool loadConfigFromFile(GameContext* ctx);

bool compareConfig(Config* config1, Config* config2) {

    bool isIdentical = true;

    // Controls

    KeyBindings* keys1 = &config1->options.controls.keys;
    KeyBindings* keys2 = &config2->options.controls.keys;

    if (keys1->fire != keys2->fire) isIdentical = false;
    if (keys1->left != keys2->left) isIdentical = false;
    if (keys1->right != keys2->right) isIdentical = false;
    if (keys1->shield != keys2->shield) isIdentical = false;
    if (keys1->thrust != keys2->thrust) isIdentical = false;

    // Debug

    DebugConfig* debug1 = &config1->debug;
    DebugConfig* debug2 = &config2->debug;

    if (debug1->onlyOutputOnChange != debug2->onlyOutputOnChange) isIdentical = false;
    if (debug1->outputFrequency != debug2->outputFrequency) isIdentical = false;
    
    #define OUTPUT(name)                                                                          \
    do {                                                                                          \
        if (debug1->poolCount.name != debug2->poolCount.name) isIdentical = false;  \
    } while (0);                                                                                            
    POOL_COUNTS(OUTPUT)

    #undef OUTPUT

    // Video

    VideoOptions* video1 = &config1->options.video;
    VideoOptions* video2 = &config2->options.video;

    if (video1->fullscreen != video2->fullscreen) isIdentical = false;
    if (video1->isMonitorSetByUser != video2->isMonitorSetByUser) isIdentical = false;
    if (video1->selectecMonitor != video2->selectecMonitor) isIdentical = false;
    if (video1->showFps != video2->showFps) isIdentical = false;
    if (video1->vSync != video2->vSync) isIdentical = false;

    return isIdentical;
}

Config getConfig(GameContext* ctx) {

    Config config;

    config.debug.onlyOutputOnChange = ctx->debug.onlyOutputOnChange;
    config.debug.outputFrequency = ctx->debug.outputFrequency;
    
    #define OUTPUT(name)                                                            \
    do {                                                                            \
            config.debug.poolCount.name = ctx->debug.poolCount.name.showInDebug;    \
    } while (0);                                                                    
    POOL_COUNTS(OUTPUT)

    #undef OUTPUT

    config.options.video.fullscreen = ctx->options.video.fullscreen;
    config.options.video.isMonitorSetByUser = ctx->options.video.isMonitorSetByUser;
    config.options.video.selectecMonitor = ctx->options.video.selectecMonitor;
    config.options.video.showFps = ctx->options.video.showFps;
    config.options.video.vSync = ctx->options.video.vSync;

    return config;
}

void initConfig(GameContext* ctx) {
    
    bool success = false;
    
    if (FileExists("./config.dat")) {
        success = loadConfigFromFile(ctx);  
    } 

    if (success) {
        printf("Successfully loaded config from file\n");
    } else {
        printf("Error: Could not load config from file, recreating file...\n");

        resetConfig(ctx);
        saveConfigToFile(ctx);
    }
}

bool loadConfigFromFile(GameContext* ctx) {
    int size;
    bool success = true;

    Config *configFromFile = (Config *)LoadFileData("./config.dat", &size);

    bool hasInvalidValues = false;

    if (configFromFile && size == sizeof(Config)) {

        // Controls

        KeyBindings* fileKeys = &configFromFile->options.controls.keys;
        KeyBindings* ctxKeys = &ctx->options.controls.keys;

        if (fileKeys->fire > 0 && fileKeys->fire < INT32_MAX) {
            ctxKeys->fire = fileKeys->fire;
        } else {
            ctxKeys->fire = KEY_BIND_FIRE_DEFAULT_VALUE;
            hasInvalidValues = true;
        }

        if (fileKeys->left > 0 && fileKeys->left < INT32_MAX) {
            ctxKeys->left = fileKeys->left;
        } else {
            ctxKeys->left = KEY_BIND_LEFT_DEFAULT_VALUE;
            hasInvalidValues = true;
        }

        if (fileKeys->right > 0 && fileKeys->right < INT32_MAX) {
            ctxKeys->right = fileKeys->right;
        } else {
            ctxKeys->right = KEY_BIND_RIGHT_DEFAULT_VALUE;
            hasInvalidValues = true;
        }

        if (fileKeys->shield > 0 && fileKeys->shield < INT32_MAX) {
            ctxKeys->shield = fileKeys->shield;
        } else {
            ctxKeys->shield = KEY_BIND_SHIELD_DEFAULT_VALUE;
            hasInvalidValues = true;
        }

        if (fileKeys->thrust > 0 && fileKeys->thrust < INT32_MAX) {
            ctxKeys->thrust = fileKeys->thrust;
        } else {
            ctxKeys->thrust = KEY_BIND_THRUST_DEFAULT_VALUE;
            hasInvalidValues = true;
        }

        // Video

        VideoOptions* fileVideoOps = &configFromFile->options.video; 
        VideoOptions* ctxVideoOps = &ctx->options.video; 

        if (fileVideoOps->fullscreen == true || fileVideoOps->fullscreen == false) {
            ctxVideoOps->fullscreen = fileVideoOps->fullscreen;
        } else {
            ctxVideoOps->fullscreen = FULLSCREEN_DEFAULT_VALUE;
            hasInvalidValues = true;
        }

        if (fileVideoOps->isMonitorSetByUser == true || fileVideoOps->isMonitorSetByUser == false) {
            ctxVideoOps->isMonitorSetByUser = fileVideoOps->isMonitorSetByUser;
        } else {
            ctxVideoOps->isMonitorSetByUser = IS_MONITOR_SET_BY_USER_DEFAULT_VALUE;
            hasInvalidValues = true;
        }

        if (fileVideoOps->selectecMonitor >= 0 || fileVideoOps->selectecMonitor < 99 ) {
            ctxVideoOps->selectecMonitor = fileVideoOps->selectecMonitor;
        } else {
            ctxVideoOps->selectecMonitor = SELECTED_MONITOR_DEFAULT_VALUE;
            hasInvalidValues = true;
        }

        if (fileVideoOps->showFps == true || fileVideoOps->showFps == false) {
            ctxVideoOps->showFps = fileVideoOps->showFps;
        } else {
            ctxVideoOps->showFps = SHOW_FPS_DEFAULT_VALUE;
            hasInvalidValues = true;
        }
        
        if (fileVideoOps->vSync == true || fileVideoOps->vSync == false) {
            ctxVideoOps->vSync = fileVideoOps->vSync;
        } else {
            ctxVideoOps->vSync = IS_V_SYNC_ENABLED_DEFAULT_VALUE;
            hasInvalidValues = true;
        }

        // Debug

        DebugConfig* fileDebug = &configFromFile->debug;
        Debug* ctxDebug = &ctx->debug;
        
        if (fileDebug->onlyOutputOnChange == true ||fileDebug->onlyOutputOnChange == false) {
            ctxDebug->onlyOutputOnChange = fileDebug->onlyOutputOnChange;
        } else {
            ctxDebug->onlyOutputOnChange = true;
            hasInvalidValues = true;
        }
        
        if (fileDebug->outputFrequency >= MIN_DEBUG_OUTPUT_FREQUENCY && fileDebug->outputFrequency <= MAX_DEBUG_OUTPUT_FREQUENCY) {
            ctxDebug->outputFrequency = fileDebug->outputFrequency;
        } else {
            ctxDebug->outputFrequency = DEFAULT_DEBUG_OUTPUT_FREQUENCY;
            hasInvalidValues = true;
        }

        #define OUTPUT(name)                                                                      \
            do {                                                                                  \
                if (fileDebug->poolCount.name == true || fileDebug->poolCount.name == false) {    \
                    ctxDebug->poolCount.name.showInDebug = fileDebug->poolCount.name;             \
                } else {                                                                          \
                    ctxDebug->poolCount.name.showInDebug = true;                                  \
                    hasInvalidValues = true;                                                      \
                }                                                                                 \
            } while (0);                                                                                                
        
            POOL_COUNTS(OUTPUT)

        #undef OUTPUT
        
    } else {
        printf("Error: Could not read config from file in loadConfigFromFile");
        success = false;
    }

    UnloadFileData((unsigned char *)configFromFile);

    if (hasInvalidValues) {
        printf("Error: Config file had invalid values, saving new config file...\n");
        saveConfigToFile(ctx);
        success = false;
    }

    return success;
}

void resetConfig(GameContext* ctx) {
    resetDebugConfig(ctx);
    resetOptionsToDefault(&ctx->options);
}

void resetDebugConfig(GameContext* ctx) {
    ctx->debug.onlyOutputOnChange = true;
    ctx->debug.outputFrequency = DEFAULT_DEBUG_OUTPUT_FREQUENCY;

    #define OUTPUT(name)                                    \
    do {                                                    \
            ctx->debug.poolCount.name.showInDebug = true;   \
    } while (0);                                            
    POOL_COUNTS(OUTPUT)

    #undef OUTPUT
}

void saveConfigToFile(GameContext* ctx) {
    Config config = getConfig(ctx);

    bool success = SaveFileData("./config.dat", &config, sizeof(config));

    if (!success) {
        printf("Error: Could not save config to file in saveConfigToFile");
    }    
}