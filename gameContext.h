#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include "raylib.h"

typedef struct Animations {
    Texture2D explosion;
}Animations;

typedef struct Sprites {
    Texture2D asteroid;
    Texture2D ship;
    Texture2D shot;
}Sprites;

#endif