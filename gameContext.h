#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include "raylib.h"
#include "asteroid.h"
#include "shooting.h"
#include "animation.h"
#include "player.h"
#include "ship.h"

typedef struct Animations {
    Texture2D explosion;
}Animations;

typedef struct Assets {
    Animations animations;
    Samples samples;
    Sprites sprites;
}Assets;

typedef struct GameContext {
    Assets assets;
    ObjectPools objectPools;
    Player player;
    Ship ship;
}GameContext;

typedef struct ObjectPools {
    AsteroidPool asteroids;
    DestroyedAsteroidPool destroyedAsteroids;
    AnimationPool explosions;
    ShotObjectPool shots;
}ObjectPools;

typedef struct Samples {
    Sound explosion;
    Sound shot;
}Samples;

typedef struct Sprites {
    Texture2D asteroid;
    Texture2D ship;
    Texture2D shot;
}Sprites;

void initGameContext(GameContext* ctx);
void initObjectPools(GameContext* ctx);
void loadAssets(GameContext* ctx);
void unloadAssets(GameContext* ctx);

#endif