#include "utils.h"
#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include "constants.h"
#include "stdio.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

void applyGForce(Vector2 position, Vector2  destination, Vector2* velocity, float maxVelocity, float minAcceleration, float maxAcceleration, float maxDistance) {
    float distanceToDestination = Vector2DistanceSqr(position, destination);
    float acceleration = scaleFloat(maxDistance, 0, minAcceleration, maxAcceleration,distanceToDestination);
    goToDestination(position, destination, velocity, maxVelocity, acceleration);
}


float scaleFloat(float oldMin, float oldMax, float newMin, float newMax, float value) {
    return (value - oldMin) / (oldMax - oldMin) * (newMax - newMin) + newMin;
}

char* getKeyName(int keyCode, char *name, size_t nameSize)
{
    const char *raylibName = GetKeyName(keyCode);

    if (raylibName && raylibName[0] != '\0') {
        snprintf(name, nameSize, "%s", raylibName);
        return name;
    }

    switch (keyCode) {
        case KEY_APOSTROPHE:    snprintf(name, nameSize, "APOSTROPHE"); break;
        case KEY_COMMA:         snprintf(name, nameSize, "COMMA"); break;
        case KEY_MINUS:         snprintf(name, nameSize, "MINUS"); break;
        case KEY_PERIOD:        snprintf(name, nameSize, "PERIOD"); break;
        case KEY_SLASH:         snprintf(name, nameSize, "SLASH"); break;

        case KEY_SEMICOLON:     snprintf(name, nameSize, "SEMICOLON"); break;
        case KEY_EQUAL:         snprintf(name, nameSize, "EQUAL"); break;

        case KEY_LEFT_BRACKET:  snprintf(name, nameSize, "LEFT BRACKET"); break;
        case KEY_BACKSLASH:     snprintf(name, nameSize, "BACKSLASH"); break;
        case KEY_RIGHT_BRACKET: snprintf(name, nameSize, "RIGHT BRACKET"); break;
        case KEY_GRAVE:         snprintf(name, nameSize, "GRAVE"); break;

        case KEY_SPACE:         snprintf(name, nameSize, "SPACE"); break;
        case KEY_ESCAPE:        snprintf(name, nameSize, "ESC"); break;
        case KEY_ENTER:         snprintf(name, nameSize, "ENTER"); break;
        case KEY_TAB:           snprintf(name, nameSize, "TAB"); break;
        case KEY_BACKSPACE:     snprintf(name, nameSize, "BACKSPACE"); break;
        case KEY_INSERT:        snprintf(name, nameSize, "INSERT"); break;
        case KEY_DELETE:        snprintf(name, nameSize, "DELETE"); break;

        case KEY_RIGHT:         snprintf(name, nameSize, "RIGHT"); break;
        case KEY_LEFT:          snprintf(name, nameSize, "LEFT"); break;
        case KEY_DOWN:          snprintf(name, nameSize, "DOWN"); break;
        case KEY_UP:            snprintf(name, nameSize, "UP"); break;
        case KEY_PAGE_UP:       snprintf(name, nameSize, "PAGE UP"); break;
        case KEY_PAGE_DOWN:     snprintf(name, nameSize, "PAGE DOWN"); break;
        case KEY_HOME:          snprintf(name, nameSize, "HOME"); break;
        case KEY_END:           snprintf(name, nameSize, "END"); break;

        case KEY_CAPS_LOCK:     snprintf(name, nameSize, "CAPS LOCK"); break;
        case KEY_SCROLL_LOCK:   snprintf(name, nameSize, "SCROLL LOCK"); break;
        case KEY_NUM_LOCK:      snprintf(name, nameSize, "NUM LOCK"); break;
        case KEY_PRINT_SCREEN:  snprintf(name, nameSize, "PRINT SCREEN"); break;
        case KEY_PAUSE:         snprintf(name, nameSize, "PAUSE"); break;

        case KEY_F1:            snprintf(name, nameSize, "F1"); break;
        case KEY_F2:            snprintf(name, nameSize, "F2"); break;
        case KEY_F3:            snprintf(name, nameSize, "F3"); break;
        case KEY_F4:            snprintf(name, nameSize, "F4"); break;
        case KEY_F5:            snprintf(name, nameSize, "F5"); break;
        case KEY_F6:            snprintf(name, nameSize, "F6"); break;
        case KEY_F7:            snprintf(name, nameSize, "F7"); break;
        case KEY_F8:            snprintf(name, nameSize, "F8"); break;
        case KEY_F9:            snprintf(name, nameSize, "F9"); break;
        case KEY_F10:           snprintf(name, nameSize, "F10"); break;
        case KEY_F11:           snprintf(name, nameSize, "F11"); break;
        case KEY_F12:           snprintf(name, nameSize, "F12"); break;

        case KEY_LEFT_SHIFT:    snprintf(name, nameSize, "LEFT SHIFT"); break;
        case KEY_LEFT_CONTROL:  snprintf(name, nameSize, "LEFT CTRL"); break;
        case KEY_LEFT_ALT:      snprintf(name, nameSize, "LEFT ALT"); break;
        case KEY_LEFT_SUPER:    snprintf(name, nameSize, "LEFT SUPER"); break;
        case KEY_RIGHT_SHIFT:   snprintf(name, nameSize, "RIGHT SHIFT"); break;
        case KEY_RIGHT_CONTROL: snprintf(name, nameSize, "RIGHT CTRL"); break;
        case KEY_RIGHT_ALT:     snprintf(name, nameSize, "RIGHT ALT"); break;
        case KEY_RIGHT_SUPER:   snprintf(name, nameSize, "RIGHT SUPER"); break;
        case KEY_KB_MENU:       snprintf(name, nameSize, "MENU"); break;

        case KEY_KP_0:          snprintf(name, nameSize, "KP 0"); break;
        case KEY_KP_1:          snprintf(name, nameSize, "KP 1"); break;
        case KEY_KP_2:          snprintf(name, nameSize, "KP 2"); break;
        case KEY_KP_3:          snprintf(name, nameSize, "KP 3"); break;
        case KEY_KP_4:          snprintf(name, nameSize, "KP 4"); break;
        case KEY_KP_5:          snprintf(name, nameSize, "KP 5"); break;
        case KEY_KP_6:          snprintf(name, nameSize, "KP 6"); break;
        case KEY_KP_7:          snprintf(name, nameSize, "KP 7"); break;
        case KEY_KP_8:          snprintf(name, nameSize, "KP 8"); break;
        case KEY_KP_9:          snprintf(name, nameSize, "KP 9"); break;
        case KEY_KP_DECIMAL:    snprintf(name, nameSize, "KP DECIMAL"); break;
        case KEY_KP_DIVIDE:     snprintf(name, nameSize, "KP DIVIDE"); break;
        case KEY_KP_MULTIPLY:   snprintf(name, nameSize, "KP MULTIPLY"); break;
        case KEY_KP_SUBTRACT:   snprintf(name, nameSize, "KP SUBTRACT"); break;
        case KEY_KP_ADD:        snprintf(name, nameSize, "KP ADD"); break;
        case KEY_KP_ENTER:      snprintf(name, nameSize, "KP ENTER"); break;
        case KEY_KP_EQUAL:      snprintf(name, nameSize, "KP EQUAL"); break;

        default:
            snprintf(name, nameSize, "UNKNOWN");
            break;
    }

    return name;
}

int getNumberOfAsteroids(int gameLevel) {
    return gameLevel + 2;
}

float getRandomFloat(float min, float max) {
    float randomFloat = (float)rand()/(float)(RAND_MAX);
    return scaleFloat(0.0f, 1.0f, min, max, randomFloat);
}

Vector2 getRandomPosition() {
    return (Vector2){GetRandomValue(SIDEBAR_WIDTH + 10, SCREEN_WIDTH - 10 - SIDEBAR_WIDTH), GetRandomValue(10, SCREEN_HEIGHT - 10)};
}

Vector2 getRandomPositionOffScreen(int size) {

    PositionChoice pos = GetRandomValue(0,3);

    const int MIN_X = SIDEBAR_WIDTH;
    const int MAX_X = SCREEN_WIDTH - SIDEBAR_WIDTH;
    const int MIN_Y = 0;
    const int MAX_Y = SCREEN_HEIGHT;

    int x = 0;
    int y = 0;

    switch (pos)
    {
    case TOP_POS:
        y = -((size / 2) - 1);
        x = GetRandomValue(MIN_X, MAX_X);
        break;

    case BOTTOM_POS:
        y = MAX_Y + ((size / 2) - 1);
        x = GetRandomValue(MIN_X, MAX_X);
        break;

    case LEFT_POS:
        y = GetRandomValue(MIN_Y, MAX_Y);
        x = MIN_X -((size / 2) - 1);
        break;
    
    case RIGHT_POS:
        y = GetRandomValue(MIN_Y, MAX_Y);
        x = MAX_X + ((size / 2) - 1);
        break;
    
    default:
        printf("Error: Invalid PosChoice in getRandomPositionOffScreen");
        break;
    }

    return (Vector2){x, y};
}

Vector2 getRandomVelocity(FloatRange range) {
    bool directionX = GetRandomValue(0,1);
    bool directionY = GetRandomValue(0,1);
    int tempVelocityX = GetRandomValue(range.min, range.max);
    int tempVelocityY = GetRandomValue(range.min, range.max);
    
    return (Vector2){directionX ? tempVelocityX : -tempVelocityX, directionY ? tempVelocityY : -tempVelocityY};
}

float getRoundness(Rectangle rect, float radiusPx) {
    float minDim = rect.width < rect.height ? rect.width : rect.height;
    return (radiusPx * 2.0f) / minDim;
}

void goToDestination(Vector2 position, Vector2  destination, Vector2* velocity, float maxVelocity, float acceleration) {
    Vector2 toDestination = Vector2Subtract(destination, position);
    Vector2 desiredVelocity = Vector2Scale(Vector2Normalize(toDestination), maxVelocity);
    Vector2 steering = Vector2Subtract(desiredVelocity, *velocity);

    float steerLength = Vector2Length(steering);

    if (steerLength > acceleration) {
        steering = Vector2Scale(Vector2Normalize(steering), acceleration);
    }

    velocity->x += steering.x * GetFrameTime();
    velocity->y += steering.y * GetFrameTime();
}

void initTitleWithPosition(TitleWithPosition* twp, char* title, Vector2 position) {
    twp->position = position;
    strcpy(twp->title, title);
}

void knockback(Vector2* targetVelocity, Vector2 forceDirection, float force) {

    forceDirection = Vector2Normalize(forceDirection);
    forceDirection = Vector2Scale(forceDirection, force);

    targetVelocity->x += forceDirection.x;
    targetVelocity->y += forceDirection.y;
}

void knockbackPoolball(Vector2 targetPosition, Vector2* targetVelocity, Vector2 forcePosition, int force) {
    Vector2 hitDirection = Vector2Subtract(targetPosition, forcePosition);
    hitDirection = Vector2Normalize(hitDirection);

    targetVelocity->x += hitDirection.x * force;
    targetVelocity->y += hitDirection.y * force;
}

void knockbackByImpact(
    Vector2 targetPosition,
    Vector2* targetVelocity,
    Vector2 forcePosition,
    Vector2 forceVelocity
) {
    Vector2 normal =
        Vector2Subtract(targetPosition, forcePosition);

    normal = Vector2Normalize(normal);

    Vector2 relativeVelocity =
        Vector2Subtract(*targetVelocity, forceVelocity);

    float speed =
        Vector2DotProduct(relativeVelocity, normal);

    if (speed < 0) {
        targetVelocity->x -= normal.x * speed;
        targetVelocity->y -= normal.y * speed;
    }
}

void playSoundPositioned(Sound sound, float positionX) {

    float minPosition = SIDEBAR_WIDTH;
    float maxPosition = SCREEN_WIDTH - SIDEBAR_WIDTH;
    float minPan = 0.7f;
    float maxPan = 0.3f;

    if (positionX < minPosition) positionX = minPosition;
    if (positionX > maxPosition) positionX = maxPosition;

    float pan = scaleFloat(minPosition, maxPosition, minPan, maxPan, positionX);

    SetSoundPan(sound, pan);
    PlaySound(sound);
}

void shake(Vector2* position , double startTime, float duration) {

    float timePassed = GetTime() - startTime;

    float minShake = scaleFloat(0.0f, duration, 4.0f, 1.0f, timePassed);
    float maxShake = scaleFloat(0.0f, duration, 12.0f, 3.0f, timePassed);

    int shakeX = GetRandomValue(minShake, maxShake);
    int shakeY = GetRandomValue(minShake, maxShake);

    if (GetRandomValue(0,1)) {
        shakeX = -shakeX;
    }

    if (GetRandomValue(0,1)) {
        shakeY = -shakeY;
    }

    position->x += shakeX; 
    position->y += shakeY; 
}

void toggleBoolCallback(void* userData) {
    bool* value = userData;

    *value = !(*value);
}

void updatePosition(Vector2* position, Vector2 velocity) {
    position->x += GetFrameTime() * velocity.x;
    position->y += GetFrameTime() * velocity.y;
}

void updateRotation(float* rotation, float rotationSpeed) {
        *rotation += GetFrameTime() * rotationSpeed;
        *rotation = fmodf(*rotation, 360.0f);

        if (*rotation < 0.0f)
        {
            *rotation += 360.0f;
        }
}