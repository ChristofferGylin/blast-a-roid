#include "ship.h"
#include "player.h"
#include "raylib.h"
#include "constants.h"

static const float SHIELD_DRAIN_RATE = 0.1f;

void updateShield(Ship* ship, Player* player) {

    if (IsKeyDown(KEY_SPACE)) {

        player->shieldPower -= GetFrameTime() * SHIELD_DRAIN_RATE;

        if (player->shieldPower <= 0) {
            player->shieldPower = 0;
            ship->isShieldActive = false;
        } else {
            ship->isShieldActive = true;
        }

    } else {
        ship->isShieldActive = false;
    }
}

void renderShield(Ship* ship) {
    if (!ship->isShieldActive) return;

    DrawCircleLinesV(ship->position, SHIELD_SIZE /2, BLUE);
}