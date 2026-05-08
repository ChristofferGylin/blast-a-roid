#ifndef ANIMATION
#define ANIMATION

#include "raylib.h"
#include "stdbool.h"

#define MAX_FRAMES 128
#define MAX_ANIMATIONS 128

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
    float rotation;
    int currentFrame;
    float frameTimer;
    bool isFinished;
}AnimationInstance;

typedef struct AnimationPoolObject {
    bool active;
    AnimationInstance aniInstance; 
}AnimationPoolObject;

typedef struct AnimationPool {
    AnimationPoolObject animations[MAX_ANIMATIONS];
    int activeCount;
}AnimationPool;

void addNewAnimation(AnimationPool* pool, Animation* animation, Vector2 position, float rotation);
void compactAnimationPool(AnimationPool* pool);
void handleFinishedAnimations(AnimationPool* pool);
void initAnimation(Animation* animation, char* spritesheetPath, const char* jsonPath, float fps, Vector2 size, bool isLoop);
void initAnimtionInstance(AnimationInstance* instance, Animation* animation, Vector2 position, float rotation);
void initAnimationPool(AnimationPool* pool);
void renderAnimation(AnimationInstance* aniInst);
void renderAnimationPool(AnimationPool* pool);
void unloadAnimation(Animation* animation);
void updateAnimation(AnimationInstance* aniInst);
void updateAnimationPool(AnimationPool* pool);

#endif