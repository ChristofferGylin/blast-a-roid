#include "raylib.h"

#ifndef SHIP_H
#define SHIP_H

#include "animation.h"

typedef struct GameContext GameContext;

typedef struct DestroyedShipPiece {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    int rotationSpeed;
    Texture2D* sprite;
}DestroyedShipPiece;

typedef struct Ship {
    AnimationInstance animation;
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool destroyed;
    bool isShieldActive;
    bool isRotateActive;
    double timeDestroyed;
    double timeRotateActivated;
    double timeSpawned;
    DestroyedShipPiece destroyedPieces[3]; 
}Ship;

void brakeShip(Vector2* velocity, float brakeFactor);
void destroyShip(GameContext* ctx, Ship* ship);
bool handleDestroyedPiecesMovement(Ship* ship);
void handleShipControls(GameContext* ctx);
void initShip(GameContext* ctx, Ship* ship);
void renderDestroyedShip(Ship* ship);
void renderShip(Ship* ship);
void resetDestroyedPieces(Ship* ship);
void resetShip(Ship* ship);
void updateShip(Ship* ship);

#endif