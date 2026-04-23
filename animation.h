#ifndef ANIMATION
#define ANIMATION

#include "raylib.h"
#include "stdbool.h"

typedef struct Animation {
    Texture2D texture;
    Vector2 size;
    Rectangle* frames;
    int frameCount;
    float fps;
    bool isLoop;
}Animation;

typedef struct AnimationInstance {
    Animation* animation;
    Vector2 position;
    int currentFrame;
    double startTime;
    bool isFinished;
}AnimationInstance;

void initAnimation(Animation* animation, char* spritesheetPath, const char* jsonPath, float fps, Vector2 size, bool isLoop);
void renderAnimation(AnimationInstance* aniInst);
void updateAnimation(AnimationInstance* aniInst);

#endif