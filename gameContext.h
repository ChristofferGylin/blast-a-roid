#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include "raylib.h"
#include "asteroid.h"
#include "shooting.h"
#include "animation.h"

typedef struct Animations {
    Texture2D explosion;
}Animations;

typedef struct ObjectPools {
    AsteroidPool asteroids;
    DestroyedAsteroidPool destroyedAsteroids;
    ShotObjectPool shots;
    AnimationPool explosions;
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

#endif