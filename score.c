#include "score.h"
#include "constants.h"
#include "raylib.h"
#include <inttypes.h>
#include <stdio.h>

void addScore(Player* player, Asteroid* ast) {
    int score = ast->level == 1 ? ASTEROID_POINTS_1 : ast->level == 2 ? ASTEROID_POINTS_2 : ASTEROID_POINTS_3;
    player->score += score;
    player->levelBonus += score * player->timeBonusMultiplier;

    updateTimeBonusMultiplier(player);
}

void resetTimeBonusMultiplier(Player* player) {
    if (player->timeBonusTimer == 0) return;

    if ((GetTime() * 1000.0) >= player->timeBonusTimer + TIME_BONUS_LIMIT) {
        player->timeBonusMultiplier = 1;
        player->timeBonusTimer = 0;
    }
}

void updateLevelBonus(Player* player) {

    if (player->levelBonus == 0) return;

    if (GetTime() >= player->levelBonusTimer + 1) {
        uint64_t newBonus = player->levelBonus - 10;

        if (newBonus < 0) {
            player->levelBonus = 0;
        } else {
            player->levelBonus = newBonus;
        }

        player->levelBonusTimer = GetTime();
    }
}

void updateTimeBonusMultiplier(Player* player) {
    player->timeBonusTimer = GetTime() * 1000.0;

    if (player->timeBonusMultiplier < 10) {
        player->timeBonusMultiplier++;
    }
}

