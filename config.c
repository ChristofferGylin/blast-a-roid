#include "config.h"
#include "debug.h"
#include "gameContext.h"
#include "raylib.h"

void loadConfigFromFile(GameContext* ctx);

void loadConfigFromFile(GameContext* ctx) {
    int size;

    Config *configFromFile = (Config *)LoadFileData("./config.dat", &size);

    bool hasInvalidValues = false;

    if (configFromFile && size == sizeof(Config)) {
        
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
                    ctx->debug.poolCount.name = configFromFile->debug.poolCount.name;                                   \
                } else {                                                                                                \
                    ctx->debug.poolCount.name = true;                                                                   \
                }                                                                                                       \
            } while (0);                                                                                                \
        POOL_COUNTS(OUTPUT)

        #undef OUTPUT
        
    } else {
        printf("Error: Could not read config from file in loadConfigFromFile");
    }

    UnloadFileData((unsigned char *)configFromFile);
}