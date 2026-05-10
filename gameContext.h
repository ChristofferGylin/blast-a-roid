#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include "bonuses.h"
#include "raylib.h"
#include "asteroid.h"
#include "enemies.h"
#include "shooting.h"
#include "animation.h"
#include "player.h"
#include "ship.h"

typedef struct Animations {
    Animation crate;
    Animation explosion;
    Animation ufo1;
}Animations;

typedef struct Samples {
    Sound explosion;
    Sound multiplier_collect;
    Sound multiplier_spawn;
    Sound shieldUp;
    Sound shot;
}Samples;

typedef struct Sprites {
    Texture2D asteroid;
    Texture2D autoShotIcon;
    Texture2D destroyedShip1;
    Texture2D destroyedShip2;
    Texture2D destroyedShip3;
    Texture2D lockIcon;
    Texture2D longShotIcon;
    Texture2D ship;
    Texture2D shot;
    Texture2D stopIcon;
    Texture2D triShotIcon;
    Texture2D enemyShot1;
}Sprites;

typedef struct Assets {
    Animations animations;
    Samples samples;
    Sprites sprites;
}Assets;

typedef struct ObjectPools {
    AsteroidPool asteroids;
    BonusObjectPool bonuses;
    DestroyedAsteroidPool destroyedAsteroids;
    EnemyObjectPool enemies;
    EnemySpawnPool spawnableEnemies;
    AnimationPool explosions;
    ShotObjectPool shots;
}ObjectPools;

typedef struct SpawningEnemies {
    double nextSpawn;
    FloatRange spawnDelay;
}SpawningEnemies;

typedef struct GameContext {
    Assets assets;
    ObjectPools objectPools;
    Player player;
    Ship ship;
    SpawningEnemies spawning;
}GameContext;

void initGameContext(GameContext* ctx);
void initObjectPools(GameContext* ctx);
void initSpawning(GameContext* ctx);
void loadAssets(GameContext* ctx);
void unloadAssets(GameContext* ctx);

#endif