#include "ship.h"
#include "player.h"
#include "raylib.h"
#include "constants.h"
#include <math.h>

static const float SHIELD_DRAIN_RATE = 0.05f;
static const Color SHIELD_GRADIENT_COLOR_1 = {126, 0, 252, 64};
static const Color SHIELD_GRADIENT_COLOR_2 = {0, 205, 252, 64};
static const Color SHIELD_LINE_COLOR = {0, 236, 252, 200};

Color color1 = SHIELD_GRADIENT_COLOR_1;
Color color2 = SHIELD_GRADIENT_COLOR_2;

float shieldLerpT = 0.0f;

void handleShield(Ship* ship, Player* player) {

    if (IsKeyDown(KEY_SPACE)) {

        player->shieldPower -= GetFrameTime() * SHIELD_DRAIN_RATE;

        if (player->shieldPower <= 0.0f) {
            player->shieldPower = 0.0f;
            ship->isShieldActive = false;
        } else {
            ship->isShieldActive = true;
        }

    } else {
        ship->isShieldActive = false;
    }
}

Color lerpColor(Color a, Color b, float t) {
    return (Color){
        (unsigned char)(a.r + (b.r - a.r) * t),
        (unsigned char)(a.g + (b.g - a.g) * t),
        (unsigned char)(a.b + (b.b - a.b) * t),
        (unsigned char)(a.a + (b.a - a.a) * t),
    };
}

void renderShield(Ship* ship) {
    if (!ship->isShieldActive) return;

    Color inner = lerpColor(SHIELD_GRADIENT_COLOR_1, SHIELD_GRADIENT_COLOR_2, shieldLerpT);
    Color outer = lerpColor(SHIELD_GRADIENT_COLOR_2, SHIELD_GRADIENT_COLOR_1, shieldLerpT);

    DrawCircleGradient(ship->position.x, ship->position.y, SHIELD_SIZE / 2, inner, outer);
    DrawCircleLinesV(ship->position, SHIELD_SIZE /2, outer);
}

void updateShieldAnimation() {
    shieldLerpT = (sinf(GetTime() * 2.0f) + 1.0f) / 2.0f;

    if (shieldLerpT >= 1.0f) {
        shieldLerpT = 1.0f;
    } else if (shieldLerpT <= 0.0f) {
        shieldLerpT = 0.0f;
    }
}