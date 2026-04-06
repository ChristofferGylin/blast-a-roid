#include "score.h"
#include "constants.h"
#include "inttypes.h"
#include <stdio.h>

void addScore(Player* player, Asteroid* ast) {
    int score = ast->level == 1 ? ASTEROID_POINTS_1 : ast->level == 2 ? ASTEROID_POINTS_2 : ASTEROID_POINTS_3;
    player->score += score;
    player->levelBonus += score * player->timeBonusMultiplier;

    printf("Score: %" PRIu64 "\n", player->score);
    printf("Level Bonus: %" PRIu64 "\n", player->levelBonus);
    printf("Time Bonus Multiplier: %d\n", player->timeBonusMultiplier);
}

void resetTimeBonusMultiplier(Player* player, double* timer) {
    if (timer == 0) return;

    if ((GetTime() * 1000) >= *timer + TIME_BONUS_LIMIT) {
        player->timeBonusMultiplier = 1;
        timer == 0;
    }
}