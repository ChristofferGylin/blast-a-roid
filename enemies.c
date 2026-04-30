#include "enemies.h"
#include "constants.h"
#include "gameContext.h"

void initUfo1(GameContext* ctx, Enemy* enemy);

void initEnemy(GameContext* ctx, Enemy* enemy, EnemyType type) {

    enemy->health = 1.0f;
    enemy->rotation = 0.0f;
    enemy->shotCount = 0;
    enemy->shotTime = GetTime();

    switch (type)
    {
    case UFO_1:
        initUfo1(ctx, enemy);
        break;
    
    default:
        break;
    }
}

void initUfo1(GameContext* ctx, Enemy* enemy) {

    float y = 50.0f;
    float velocity = 100;

    enemy->destinaton = (Vector2){SCREEN_WIDTH + 10, y};
    enemy->position = (Vector2){-10, y};
    enemy->type = UFO_1;
    enemy->velocity = velocity;
    enemy->visualType = VISUAL_ANIMATION;
    enemy->animation = &ctx->assets.animations.ufo1;
}