#include <stdbool.h>
#include "raylib.h"
#include "utils.h"
#include "constants.h"
#include "fader.h"

const float FADE_RATE = 0.75f;

bool fadeOut(float* fadeValue) {
    if (*fadeValue >= 1.0f) {
        return true;
    }

    *fadeValue += GetFrameTime() * FADE_RATE;

    if (*fadeValue > 1.0f) {
        *fadeValue = 1.0f;
    }

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, *fadeValue));

    if (*fadeValue == 1.0f) {
        return true;
    } else {
        return false;
    }
}

bool fadeIn(float* fadeValue) {
    if (*fadeValue <= 0.0f) {
        return true;
    }

    *fadeValue -= GetFrameTime() * FADE_RATE;

    if (*fadeValue < 0.0f) {
        *fadeValue = 0.0f;
    }

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, *fadeValue));

    if (*fadeValue == 0.0f) {
        return true;
    } else {
        return false;
    }
}
