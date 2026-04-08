#include <stdbool.h>
#include "raylib.h"
#include "utils.h"
#include "constants.h"
#include "fader.h"

const int FADE_TIME = 2;

bool fader(FaderArgs* args) {
    
    bool isComplete = false;
    
    if (args->fadeIn) {
        if (args->fadeValue == 0.0f) return true;
        if (args->fadeValue == 1.0f) {
            args->timer = GetTime();
        }

        args->fadeValue = scaleFloat(args->timer, args->timer + FADE_TIME, 1.0f, 0.0f, GetTime());

        if (args->fadeValue <= 0.0f) {
            args->fadeValue = 0.0f;
            isComplete = true;
        }
    } else {
        if (args->fadeValue == 0.0f) {
            args->timer = GetTime();
        }

        args->fadeValue = scaleFloat(args->timer, args->timer + FADE_TIME, 0.0f, 1.0f, GetTime());
        if (args->fadeValue >= 1.0f) {
            args->fadeValue = 1.0f;
            isComplete = true;
        }
    }

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, args->fadeValue));
    return isComplete;
}