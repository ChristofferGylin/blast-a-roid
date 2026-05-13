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

void brakeShip(Vector2* velocity, float brakeFactor);
void destroyShip(GameContext* ctx);
void handleDestroyedPiecesMovement(Ship* ship);
void handleShipControls(GameContext* ctx);
void initShip(GameContext* ctx);
void renderDestroyedShip(Ship* ship);
void renderShip(GameContext* ctx);
void resetDestroyedPieces(Ship* ship);
void resetShip(Ship* ship);

#endif