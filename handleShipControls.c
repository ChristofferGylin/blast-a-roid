#include "raylib.h"
#include "ship.h"
#include "constants.h"
#include <math.h>
#include "outOfBoundsCheck.h"


void handleShipControls(Ship *ship) 
{

    if (ship->destroyed) return;

    if (IsKeyDown(KEY_A))
    {
            ship->rotation -= GetFrameTime() * ROTATION_SPEED;
    }

    if (IsKeyDown(KEY_D))
    {
            ship->rotation += GetFrameTime() * ROTATION_SPEED;
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
    }

    ship->position.x += GetFrameTime() * ship->velocity.x;
    ship->position.y += GetFrameTime() * ship->velocity.y;

    outOfBoundsCheck(&ship->position, SHIP_SIZE);
}