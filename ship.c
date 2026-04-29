#include "ship.h"
#include "shield.h"
#include "raylib.h"
#include "gameContext.h"
#include "constants.h"

void renderShip(GameContext* ctx) {
    if (!ctx->ship.destroyed) {
        DrawTexturePro(
            ctx->assets.sprites.ship,
            (Rectangle){0, 0, ctx->assets.sprites.ship.width, ctx->assets.sprites.ship.height},
            (Rectangle){ctx->ship.position.x, ctx->ship.position.y, SHIP_SIZE, SHIP_SIZE},
            (Vector2){ SHIP_SIZE / 2.0f, SHIP_SIZE / 2.0f},
            ctx->ship.rotation,
            WHITE
        );
        renderShield(&ctx->ship);    
    }
}

void resetShip(Ship* ship) {
    ship->destroyed = false;
    ship->position.x = GetScreenWidth() / 2;
    ship->position.y = GetScreenHeight() / 2;
    ship->rotation = 0;
    ship->velocity.x = 0;
    ship->velocity.y = 0;
}