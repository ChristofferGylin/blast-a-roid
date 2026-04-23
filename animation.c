#include <math.h>
#include "cjson/cJSON.h"
#include "animation.h"

void initAnimation(Animation* animation, char* spritesheetPath, const char* jsonPath, float fps, Vector2 size, bool isLoop) {
    char* jsonText = LoadFileText(jsonPath);

    cJSON* root = cJSON_Parse(jsonText);
    cJSON* framesObj = cJSON_GetObjectItem(root, "frames");

    cJSON* frameEntry = NULL;

    Rectangle frames[MAX_FRAMES];

    cJSON_ArrayForEach(frameEntry, framesObj) {
        cJSON* frame = cJSON_GetObjectItem(frameEntry, "frame");

        if (!frame) continue;

        Rectangle f;

        f.x = (float)cJSON_GetObjectItem(frame, "x")->valuedouble;
        f.y = (float)cJSON_GetObjectItem(frame, "y")->valuedouble;
        f.width = (float)cJSON_GetObjectItem(frame, "w")->valuedouble;
        f.height = (float)cJSON_GetObjectItem(frame, "h")->valuedouble;

        if (animation->frameCount < MAX_FRAMES) {
            frames[animation->frameCount++] = f;
        }
    }

    animation->texture = LoadTexture(spritesheetPath);
    animation->size = size;
    animation->frames = frames;
    animation->fps = fps;
    animation->isLoop = isLoop;

    cJSON_Delete(root);
    UnloadFileText(jsonText);

}

void renderAnimation(AnimationInstance* aniInst) {
    DrawTexturePro(
        aniInst->animation->texture,
        aniInst->animation->frames[aniInst->currentFrame],
        (Rectangle){aniInst->position.x, aniInst->position.y, aniInst->animation->size.x, aniInst->animation->size.y},
        (Vector2){aniInst->animation->size.x / 2, aniInst->animation->size.y / 2},
        0,
        RAYWHITE
    );
}

void addNewAnimation(AnimationPool* pool, Animation* animation, Vector2 position, Vector2 size) {
    
    if (pool->activeCount >= pool->capacity) {
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
    aniInstance.isFinished = false;
    aniInstance.position = position;
    aniInstance.startTime = GetTime();

    pool->animations[pool->activeCount].aniInstance = aniInstance;
    pool->animations[pool->activeCount].active = true;
    pool->activeCount++;
} 

void updateAnimation(AnimationInstance* aniInst) {
    aniInst->currentFrame = round((GetTime() - aniInst->startTime) * aniInst->animation->fps);

    if (aniInst->currentFrame > aniInst->animation->frameCount) {

        if (aniInst->animation->isLoop) {
            aniInst->currentFrame = 0;
        } else {
            aniInst->currentFrame = 0;
            aniInst->isFinished = true;
        }
    }
}