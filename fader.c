#include <stdbool.h>
#include "raylib.h"
#include "utils.h"
#include "constants.h"

double timer = 0.0;
const int FADE_TIME = 1;
float fadeValue = 1.0f;

void fader(bool fadeIn) {
    if (fadeIn) {
        if (fadeValue == 0.0f) return;
        if (fadeValue == 1.0f) {
            timer = GetTime();
        }

        fadeValue = scaleFloat(timer, timer + FADE_TIME, 0.0f, 1.0f, GetTime());

        if (fadeValue < 0.0f) {
            fadeValue = 0.0f;
        }
    } else {
        if (fadeValue == 0.0f) {
            timer = GetTime();
        }

        fadeValue = scaleFloat(timer, timer + FADE_TIME, 1.0f, 0.0f, GetTime()); 
    }

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, fadeValue));
}