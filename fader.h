#ifndef FADER_H
#define FADER_H

typedef struct FaderArgs {
    bool fadeIn;
    double timer;
    float fadeValue;
}FaderArgs;

bool fader(FaderArgs* args);

#endif