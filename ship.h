#include "raylib.h"

#ifndef SHIP_H
#define SHIP_H

typedef struct GameContext GameContext;

typedef struct DestroyedShipPiece {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    int rotationSpeed;
    Texture2D* sprite;
}DestroyedShipPiece;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool destroyed;
    bool isShieldActive;
    double timeDestroyed;
    DestroyedShipPiece destroyedPieces[3]; 
}Ship;

void handleShipControls(Ship *ship);
void initShip(GameContext* ctx);
void renderDestroyedShip(Ship* ship);
void renderShip(GameContext* ctx);
void resetShip(Ship* ship);

#endif