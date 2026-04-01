#include "ship.h"
#include "constants.h"

void handleShipMovement(Ship ship) 
{
    if (IsKeyDown(KEY_A))
        {
            ship.rotation -= GetFrameTime() * ROTATION_SPEED;
        }

        if (IsKeyDown(KEY_D))
        {
            ship.rotation += GetFrameTime() * ROTATION_SPEED;
        }

        ship.rotation = fmodf(ship.rotation, 360.0f);

        if (ship.rotation < 0.0f)
        {
            ship.rotation += 360.0f;
        }

        if (IsKeyDown(KEY_W))
        {
            float radians = (ship.rotation - 90.0f) * (PI / 180.0f);

            ship.velocity.x += cosf(radians) * THRUST_FACTOR;
            ship.velocity.y += sinf(radians) * THRUST_FACTOR;

            if (ship.velocity.x < -MAX_VELOCITY)
            {
                ship.velocity.x = -MAX_VELOCITY;
            } else if (ship.velocity.x > MAX_VELOCITY)
            {
                ship.velocity.x = MAX_VELOCITY;
            }

            if (ship.velocity.y < -MAX_VELOCITY)
            {
                ship.velocity.y = -MAX_VELOCITY;
            } else if (ship.velocity.y > MAX_VELOCITY)
            {
                ship.velocity.y = MAX_VELOCITY;
            }
        }

        ship.position.x += GetFrameTime() * ship.velocity.x;
        ship.position.y += GetFrameTime() * ship.velocity.y;

        if (ship.position.x < 0.0f - ship.sprite.width)
        {
            ship.position.x = GetScreenWidth() + ship.sprite.width;
        } else if (ship.position.x > GetScreenWidth() + ship.sprite.width)
        {
            ship.position.x = 0.0f - ship.sprite.width; 
        }

        if (ship.position.y < 0.0f - ship.sprite.height)
        {
            ship.position.y = GetScreenHeight() + ship.sprite.height;
        } else if (ship.position.y > GetScreenHeight() + ship.sprite.height)
        {
            ship.position.y = 0.0f - ship.sprite.height; 
        }
}