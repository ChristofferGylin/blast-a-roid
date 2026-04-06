#include "score.h"
#include "constants.h"
#include "raylib.h"
#include <inttypes.h>
#include <stdio.h>

void addScore(Player* player, Asteroid* ast) {
    int score = ast->level == 1 ? ASTEROID_POINTS_1 : ast->level == 2 ? ASTEROID_POINTS_2 : ASTEROID_POINTS_3;
    player->score += score;
    player->levelBonus += score * player->timeBonusMultiplier;

    printf("Score: %" PRIu64 "\n", player->score);
    printf("Level Bonus: %" PRIu64 "\n", player->levelBonus);
    printf("Time Bonus Multiplier: %d\n", player->timeBonusMultiplier);

    updateTimeBonusMultiplier(player);
}

void resetTimeBonusMultiplier(Player* player) {
    if (player->timeBonusTimer == 0) return;

    if ((GetTime() * 1000.0) >= player->timeBonusTimer + TIME_BONUS_LIMIT) {
        player->timeBonusMultiplier = 1;
        player->timeBonusTimer = 0;
    }
}

void updateTimeBonusMultiplier(Player* player) {
    player->timeBonusTimer = GetTime() * 1000.0;

    if (player->timeBonusMultiplier < 10) {
        player->timeBonusMultiplier++;
    }
}