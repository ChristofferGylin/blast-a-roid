#include "cjson/cJSON.h"
#include "animation.h"

#define MAX_FRAMES 128

void initAnimation(Animation* animation, Texture2D spritesheet, const char* jsonPath, float fps) {
    char* jsonText = LoadFileText(jsonPath);

    cJSON* root = cJSON_Parse(jsonText);
    cJSON* framesObj = cJSON_GetObjectItem(root, "frames");

    cJSON* frameEntry = NULL;

    Frame frames[MAX_FRAMES];

    cJSON_ArrayForEach(frameEntry, framesObj) {
        cJSON* frame = cJSON_GetObjectItem(frameEntry, "frame");

        if (!frame) continue;

        Frame f;

        f.rect.x = (float)cJSON_GetObjectItem(frame, "x")->valuedouble;
        f.rect.y = (float)cJSON_GetObjectItem(frame, "y")->valuedouble;
        f.rect.width = (float)cJSON_GetObjectItem(frame, "w")->valuedouble;
        f.rect.height = (float)cJSON_GetObjectItem(frame, "h")->valuedouble;

        if (animation->frameCount < MAX_FRAMES) {
            frames[animation->frameCount++] = f;
        }
    }

    animation->currentFrame = 0;
    animation->frames = frames;
    animation->fps = fps;
    animation->startTime = 0.0;
    animation->texture = spritesheet;
    
    cJSON_Delete(root);
    UnloadFileText(jsonText);

}