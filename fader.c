#include <stdbool.h>
#include "raylib.h"
#include "utils.h"
#include "constants.h"
#include "fader.h"

const int FADE_TIME = 2;
double timer = 0.0;
float fadeValue = 1.0f;

bool fader(bool fadeIn) {
    
    bool isComplete = false;
    
    if (fadeIn) {
        if (fadeValue == 0.0f) return true;
        if (fadeValue == 1.0f) {
            timer = GetTime();
        }

        fadeValue = scaleFloat(timer, timer + FADE_TIME, 1.0f, 0.0f, GetTime());

        if (fadeValue < 0.0f) {
            fadeValue = 0.0f;
        }
    } else {
        if (fadeValue == 0.0f) {
            timer = GetTime();
        }

        fadeValue = scaleFloat(timer, timer + FADE_TIME, 0.0f, 1.0f, GetTime());
        if (fadeValue >= 1.0f) {
            fadeValue = 1.0f;
            isComplete = true;
        }
    }

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, fadeValue));
    return isComplete;
}