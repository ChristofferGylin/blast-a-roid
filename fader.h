#ifndef FADER_H
#define FADER_H

typedef struct Fader {
    bool isFadeInComplete;
    bool isFadeOutComplete;
    float fadeInValue;
    float fadeOutValue;
}Fader;

bool fadeIn(float* fadeValue);
bool fadeOut(float* fadeValue);

#endif