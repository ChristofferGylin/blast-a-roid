#ifndef FADER_H
#define FADER_H

typedef struct FaderArgs {
    bool fadeIn;
    bool fadeComplete;
    double timer;
    float fadeValue;
}FaderArgs;

bool fadeIn(float fadeValue);
bool fadeOut(float fadeValue);
void fader(FaderArgs* args);
void initFaderArgs(FaderArgs* args);

#endif