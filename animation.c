#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cjson/cJSON.h"
#include "animation.h"

void addNewAnimation(AnimationPool* pool, Animation* animation, Vector2 position, float rotation) {
    
    if (pool->activeCount >= MAX_ANIMATIONS) {
        printf("Error: Memory overflow in addNewAnimation\n");
        return;        
    }

    if (pool->animations[pool->activeCount].active) {
        printf("Error: Could not add new animation, index already in use in addNewAnimation\n");
        return;
    }
    
    AnimationInstance aniInstance;

    aniInstance.animation = animation;
    aniInstance.currentFrame = 0;
    aniInstance.frameTimer = 0.0f;
    aniInstance.isFinished = false;
    aniInstance.position = position;
    aniInstance.rotation = rotation;

    pool->animations[pool->activeCount].aniInstance = aniInstance;
    pool->animations[pool->activeCount].active = true;
    pool->activeCount++;
}

void compactAnimationPool(AnimationPool* pool) {
    int write = 0;

    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->animations[i].active) {
            if (write != i) {
                pool->animations[write] = pool->animations[i];
            }
            write++;
        }
    }

    for (int i = write; i < pool->activeCount; i++) {
        pool->animations[i].active = false;
    }

    pool->activeCount = write;
}

void handleFinishedAnimations(AnimationPool* pool) {

    if (pool->activeCount == 0) return;
    
    bool hasChanges = false;
    
    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->animations[i].aniInstance.isFinished) {
            pool->animations[i].active = false;
            hasChanges = true;
        }
    }

    if (hasChanges) compactAnimationPool(pool);
}

void initAnimation(Animation* animation, char* spritesheetPath, const char* jsonPath, float fps, Vector2 size, bool isLoop) {
    char* jsonText = LoadFileText(jsonPath);

    cJSON* root = cJSON_Parse(jsonText);
    cJSON* framesObj = cJSON_GetObjectItem(root, "frames");

    cJSON* frameEntry = NULL;

    animation->frames = malloc(sizeof(Rectangle) * MAX_FRAMES);
    animation->frameCount = 0;

    cJSON_ArrayForEach(frameEntry, framesObj) {
        cJSON* frame = cJSON_GetObjectItem(frameEntry, "frame");

        if (!frame) continue;

        Rectangle f;

        f.x = (float)cJSON_GetObjectItem(frame, "x")->valuedouble;
        f.y = (float)cJSON_GetObjectItem(frame, "y")->valuedouble;
        f.width = (float)cJSON_GetObjectItem(frame, "w")->valuedouble;
        f.height = (float)cJSON_GetObjectItem(frame, "h")->valuedouble;

        if (animation->frameCount < MAX_FRAMES) {
            animation->frames[animation->frameCount++] = f;
        }
    }

    animation->texture = LoadTexture(spritesheetPath);
    SetTextureFilter(animation->texture, TEXTURE_FILTER_POINT);
    animation->size = size;
    animation->fps = fps;
    animation->isLoop = isLoop;

    cJSON_Delete(root);
    UnloadFileText(jsonText);

}

void initAnimationPool(AnimationPool* pool) {
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        pool->animations[i].active = false;
    }

    pool->activeCount = 0;
}

void renderAnimation(AnimationInstance* aniInst) {
    DrawTexturePro(
        aniInst->animation->texture,
        aniInst->animation->frames[aniInst->currentFrame],
        (Rectangle){round(aniInst->position.x),  round(aniInst->position.y), round(aniInst->animation->size.x), round(aniInst->animation->size.y)},
        (Vector2){aniInst->animation->size.x / 2, aniInst->animation->size.y / 2},
        aniInst->rotation,
        RAYWHITE
    );
}

void renderAnimationPool(AnimationPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->animations[i].active || pool->animations[i].aniInstance.isFinished) continue;

        renderAnimation(&pool->animations[i].aniInstance);
    }
}

void unloadAnimation(Animation* animation) {
    UnloadTexture(animation->texture);
    free(animation->frames);
}

void updateAnimation(AnimationInstance* aniInst) {

    aniInst->frameTimer += GetFrameTime();

    if (aniInst->frameTimer >= (1.0f / aniInst->animation->fps)) {
        aniInst->frameTimer = 0.0f;
        aniInst->currentFrame++;

        if ((aniInst->currentFrame >= aniInst->animation->frameCount) && !aniInst->animation->isLoop) {
            aniInst->isFinished = true;
        } else if ((aniInst->currentFrame >= aniInst->animation->frameCount) && aniInst->animation->isLoop) {
            aniInst->currentFrame = 0;
        }
    }
}

void updateAnimationPool(AnimationPool* pool) {
    
    if (pool->activeCount == 0) return;
    
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->animations[i].active || pool->animations[i].aniInstance.isFinished) continue;

        updateAnimation(&pool->animations[i].aniInstance);
    }
}