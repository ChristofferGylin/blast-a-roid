#ifndef ENEMIES_H
#define ENEMIES_H

#include "animation.h"
#include "raylib.h"
#include "constants.h"
#include <stdbool.h>
#include "shooting.h"
#include "utils.h"

#define MAX_ENEMIES 32

typedef struct GameContext GameContext;

typedef enum EnemyType {
    UFO_1
}EnemyType;

typedef enum VisualType {
    VISUAL_SPRITE,
    VISUAL_ANIMATION
}VisualType;

typedef struct Enemy {
    float acceleration;
    Vector2 position;
    Vector2 destination;
    float rotation;
    Vector2 velocity;
    float maxVelocity;
    bool isMoveable;
    int health;
    int size;
    double lastReaction;
    double spawnTime;
    float reactionTime;
    ShootingProperties shooting;
    EnemyType type;
    VisualType visualType;
    union {
        Texture2D* sprite;
        AnimationInstance animation;
    };

}Enemy;

typedef struct EnemyPoolObject {
    bool active;
    Enemy enemy; 
}EnemyPoolObject;

typedef struct EnemyObjectPool {
    EnemyPoolObject enemies[MAX_ENEMIES];
    int activeCount;
}EnemyObjectPool;

typedef struct EnemySpawnOption {
    EnemyType type;
    int weight;
    int count;
    int maxCount;
}EnemySpawnOption;

typedef struct EnemySpawnPool {
    const EnemySpawnOption* options;
    int count;
    FloatRange spawnDelay;
}EnemySpawnPool;

void addNewEnemy(GameContext* ctx, EnemyType type);
void handleEnemiesHitDetection(GameContext* ctx);
void handleEnemiesMovement(GameContext* ctx);
void initEnemyPool(EnemyObjectPool* pool);
void removeEnemy(EnemyObjectPool* pool, Enemy* enemy);
void renderEnemies(EnemyObjectPool* pool);
void updateEnemies(GameContext* ctx);

#endif