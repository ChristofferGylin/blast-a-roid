// #include "raylib.h"
// #include <math.h>
// #include "asteroid.h"

// void handleAsteroidsMovement(AsteroidPool* pool) 
// {

//     int spriteWidth = 32;

//     for (int i = 0; i < pool->activeCount; i++) {
//         Asteroid* ast = &pool->asteroids[pool->active[i]];

//         if (ast->destroyed) continue;

//         ast->rotation += GetFrameTime() * ast->rotationSpeed;
//         ast->rotation = fmodf(ast->rotation, 360.0f);

//         if (ast->rotation < 0.0f)
//         {
//             ast->rotation += 360.0f;
//         }
        
//         ast->position.x += GetFrameTime() * ast->velocity.x;
//         ast->position.y += GetFrameTime() * ast->velocity.y;

//         if (ast->position.x < 0.0f - spriteWidth)
//         {
//             ast->position.x = GetScreenWidth() + spriteWidth;
//         } else if (ast->position.x > GetScreenWidth() + spriteWidth)
//         {
//             ast->position.x = 0.0f - spriteWidth; 
//         }

//         if (ast->position.y < 0.0f - spriteWidth)
//         {
//             ast->position.y = GetScreenHeight() + spriteWidth;
//         } else if (ast->position.y > GetScreenHeight() + spriteWidth)
//         {
//             ast->position.y = 0.0f - spriteWidth; 
//         }
//     }
// }