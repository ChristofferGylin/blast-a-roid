#ifndef FADER_H
#define FADER_H

typedef struct FaderArgs {
    bool fadeIn;
    bool fadeComplete;
    double timer;
    float fadeValue;
}FaderArgs;

void fader(FaderArgs* args);
void initFaderArgs(FaderArgs* args);

#endif