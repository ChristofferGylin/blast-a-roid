#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include "raylib.h"
#include "asteroid.h"
#include "shooting.h"
#include "animation.h"
#include "player.h"
#include "ship.h"

typedef struct Animations {
    Animation explosion;
}Animations;

typedef struct Samples {
    Sound explosion;
    Sound multiplier_collect;
    Sound multiplier_spawn;
    Sound shot;
}Samples;

typedef struct Sprites {
    Texture2D asteroid;
    Texture2D destroyedShip1;
    Texture2D destroyedShip2;
    Texture2D destroyedShip3;
    Texture2D ship;
    Texture2D shot;
}Sprites;

typedef struct Assets {
    Animations animations;
    Samples samples;
    Sprites sprites;
}Assets;

typedef struct ObjectPools {
    AsteroidPool asteroids;
    DestroyedAsteroidPool destroyedAsteroids;
    AnimationPool explosions;
    ShotObjectPool shots;
}ObjectPools;

typedef struct GameContext {
    Assets assets;
    ObjectPools objectPools;
    Player player;
    Ship ship;
}GameContext;



void initGameContext(GameContext* ctx);
void initObjectPools(GameContext* ctx);
void loadAssets(GameContext* ctx);
void unloadAssets(GameContext* ctx);

#endif