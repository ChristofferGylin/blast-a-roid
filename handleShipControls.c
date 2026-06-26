#include "raylib.h"
#include "ship.h"
#include "constants.h"
#include <math.h>
#include "outOfBoundsCheck.h"
#include "gameContext.h"

void brakeShip(Vector2* velocity, float brakeFactor) {

    float speed = sqrtf(
        velocity->x * velocity->x +
        velocity->y * velocity->y
    );

    if (speed > 0.01f) {
        float dirX = velocity->x / speed;
        float dirY = velocity->y / speed;

        velocity->x -= dirX * brakeFactor;
        velocity->y -= dirY * brakeFactor;

        float newSpeed = sqrtf(
            velocity->x * velocity->x + 
            velocity->y * velocity->y 
        );

        if (newSpeed > speed) {
            velocity->x = 0;
            velocity->y = 0;
        }
    }
}

void handleShipControls(GameContext* ctx) 
{

    Ship* ship = &ctx->ship;

    if (ship->destroyed) return;

    const float BRAKE_FACTOR = 2.0f;
    const float MAX_VELOCITY = 300.0f;
    const int NUDGE_DELAY = 50;
    const double NOW_MILLIS = GetTime() * 1000;
    const float ROTATION_NUDGE_SPEED = 200.0f;
    const float ROTATION_SPEED = 270.0f;
    const float THRUST_FACTOR = 2.5f;

    bool nudgeActive = NOW_MILLIS < ship->timeRotateActivated + NUDGE_DELAY;

    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D)) {
        ship->isRotateActive = true;
        ship->timeRotateActivated = NOW_MILLIS;
    } else {
        if (IsKeyDown(KEY_A) && !nudgeActive) {
            ship->rotation -= GetFrameTime() * ROTATION_SPEED;
        }

        if (IsKeyDown(KEY_D) && !nudgeActive) {
            ship->rotation += GetFrameTime() * ROTATION_SPEED;
        }
    }

    if (ship->isRotateActive && IsKeyReleased(KEY_A)) {
        ship->isRotateActive = false;

        if (nudgeActive) {
            ship->rotation -= GetFrameTime() * ROTATION_NUDGE_SPEED;
        }
    }

    if (ship->isRotateActive && IsKeyReleased(KEY_D)) {
        ship->isRotateActive = false;

        if (nudgeActive) {
            ship->rotation += GetFrameTime() * ROTATION_NUDGE_SPEED;
        }
    }

    ship->rotation = fmodf(ship->rotation, 360.0f);

    if (ship->rotation < 0.0f)
    {
        ship->rotation += 360.0f;
    }

    if (IsKeyDown(KEY_W))
    {
        float radians = (ship->rotation - 90.0f) * (PI / 180.0f);

        ship->velocity.x += cosf(radians) * THRUST_FACTOR;
        ship->velocity.y += sinf(radians) * THRUST_FACTOR;

        if (ship->velocity.x < -MAX_VELOCITY)
        {
            ship->velocity.x = -MAX_VELOCITY;
        } else if (ship->velocity.x > MAX_VELOCITY)
        {
            ship->velocity.x = MAX_VELOCITY;
        }

        if (ship->velocity.y < -MAX_VELOCITY)
        {
            ship->velocity.y = -MAX_VELOCITY;
        } else if (ship->velocity.y > MAX_VELOCITY)
        {
            ship->velocity.y = MAX_VELOCITY;
        }
    } else if (ctx->player.powerups.autoStop) {
        brakeShip(&ship->velocity, BRAKE_FACTOR);
    }

    Vector2 velocity = applySupernovaEffects(ctx, &ship->position, ship->velocity);

    ship->position.x += GetFrameTime() * velocity.x;
    ship->position.y += GetFrameTime() * velocity.y;

    handleOutOfBounds(&ship->position, SHIP_SIZE);
}