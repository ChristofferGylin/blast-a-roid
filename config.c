#include <stdio.h>

#include "config.h"
#include "debug.h"
#include "gameContext.h"
#include "options.h"
#include "raylib.h"

bool loadConfigFromFile(GameContext* ctx);

bool compareConfig(Config* config1, Config* config2) {

    bool isIdentical = true;

    if (config1->debug.onlyOutputOnChange != config2->debug.onlyOutputOnChange) isIdentical = false;
    if (config1->debug.outputFrequency != config2->debug.outputFrequency) isIdentical = false;
    
    #define OUTPUT(name)                                                                          \
    do {                                                                                          \
        if (config1->debug.poolCount.name != config2->debug.poolCount.name) isIdentical = false;  \
    } while (0);                                                                                            
    POOL_COUNTS(OUTPUT)

    #undef OUTPUT

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

        if (configFromFile->options.video.showFps == true || configFromFile->options.video.showFps == false) {
            ctx->options.video.showFps = configFromFile->options.video.showFps;
        } else {
            ctx->options.video.showFps = false;
            hasInvalidValues = true;
        }
        
        if (configFromFile->debug.onlyOutputOnChange == true ||configFromFile->debug.onlyOutputOnChange == false) {
            ctx->debug.onlyOutputOnChange = configFromFile->debug.onlyOutputOnChange;
        } else {
            ctx->debug.onlyOutputOnChange = true;
            hasInvalidValues = true;
        }
        
        if (configFromFile->debug.outputFrequency >= MIN_DEBUG_OUTPUT_FREQUENCY && configFromFile->debug.outputFrequency <= MAX_DEBUG_OUTPUT_FREQUENCY) {
            ctx->debug.outputFrequency = configFromFile->debug.outputFrequency;
        } else {
            ctx->debug.outputFrequency = DEFAULT_DEBUG_OUTPUT_FREQUENCY;
            hasInvalidValues = true;
        }

        #define OUTPUT(name)                                                                                            \
            do {                                                                                                        \
                if (configFromFile->debug.poolCount.name == true || configFromFile->debug.poolCount.name == false) {    \
                    ctx->debug.poolCount.name.showInDebug = configFromFile->debug.poolCount.name;                       \
                } else {                                                                                                \
                    ctx->debug.poolCount.name.showInDebug = true;                                                       \
                    hasInvalidValues = true;                                                                            \
                }                                                                                                       \
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