#include <stdbool.h>
#include "raylib.h"
#include "utils.h"
#include "constants.h"
#include "fader.h"

const int FADE_TIME = 2;
const float FADE_RATE = 5.0f;

void fader(FaderArgs* args) {
    if (args->fadeIn) {
        if (args->fadeValue == 0.0f) return;
        if (args->fadeValue == 1.0f) {
            args->timer = GetTime();
            args->fadeComplete = false;
        }

        args->fadeValue = scaleFloat(args->timer, args->timer + FADE_TIME, 1.0f, 0.0f, GetTime());

        if (args->fadeValue <= 0.0f) {
            args->fadeValue = 0.0f;
            args->fadeComplete = true;
        }
    } else {
        if (args->fadeValue == 0.0f) {
            args->timer = GetTime();
            args->fadeComplete = false;
        }

        args->fadeValue = scaleFloat(args->timer, args->timer + FADE_TIME, 0.0f, 1.0f, GetTime());
        if (args->fadeValue >= 1.0f) {
            args->fadeValue = 1.0f;
            args->fadeComplete = true;
        }
    }

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, args->fadeValue));
}

void initFaderArgs(FaderArgs* args) {
    args->fadeComplete = false;
    args->fadeIn = true;
    args->fadeValue = 1.0f;
    args->timer = 0;
}

bool fadeOut(float fadeValue) {
    if (fadeValue >= 1.0f) {
        return true;
    }

    fadeValue += GetFrameTime() * FADE_RATE;

    if (fadeValue > 1.0f) {
        fadeValue = 1.0f;
    }

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, fadeValue));

    if (fadeValue == 1.0f) {
        return true;
    } else {
        return false;
    }
}

bool fadeIn(float fadeValue) {
    if (fadeValue <= 0.0f) {
        return true;
    }

    fadeValue -= GetFrameTime() * FADE_RATE;

    if (fadeValue < 0.0f) {
        fadeValue = 0.0f;
    }

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, fadeValue));

    if (fadeValue == 0.0f) {
        return true;
    } else {
        return false;
    }
}
