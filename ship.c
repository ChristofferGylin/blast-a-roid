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

void renderDestroyedShip(Ship* ship) {
    for (int i = 0; i < 3; i++) {
        DestroyedShipPiece* piece = &ship->destroyedPieces[i];
        Texture2D sprite = *ship->destroyedPieces[i].sprite;

        DrawTexturePro(
            sprite,
            (Rectangle){0, 0, sprite.width, sprite.height},
            (Rectangle){piece->position.x, piece->position.y, sprite.width, sprite.height},
            (Vector2){ sprite.width / 2.0f, sprite.height / 2.0f },
            piece->rotation,
            WHITE
        );
    }
}

void initShip(GameContext* ctx) {
    Ship* ship = &ctx->ship;

    ship->destroyed = false;
    ship->timeDestroyed = 0.0;
    ship->position.x = GetScreenWidth() / 2;
    ship->position.y = GetScreenHeight() / 2;
    ship->rotation = 0;
    ship->velocity.x = 0;
    ship->velocity.y = 0;

    DestroyedShipPiece destroyedBase = {
        (Vector2){0, 0},
        (Vector2){0, 0},
        0,
    };

    ship->destroyedPieces[0] = destroyedBase;
    ship->destroyedPieces[0].sprite = &ctx->assets.sprites.destroyedShip1;

    ship->destroyedPieces[1] = destroyedBase;
    ship->destroyedPieces[1].sprite = &ctx->assets.sprites.destroyedShip2;

    ship->destroyedPieces[2] = destroyedBase;
    ship->destroyedPieces[2].sprite = &ctx->assets.sprites.destroyedShip2;
}

void resetShip(Ship* ship) {
    ship->destroyed = false;
    ship->position.x = GetScreenWidth() / 2;
    ship->position.y = GetScreenHeight() / 2;
    ship->rotation = 0;
    ship->velocity.x = 0;
    ship->velocity.y = 0;
}