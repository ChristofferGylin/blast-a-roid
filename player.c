#include "player.h"
#include "asteroid.h"
#include "constants.h"
#include <stdio.h>
#include <inttypes.h>
void addScore(Player* player, Asteroid* ast) {
    int score = ast->level == 1 ? ASTEROID_POINTS_1 : ast->level == 2 ? ASTEROID_POINTS_2 : ASTEROID_POINTS_3;
    player->score += score;
    player->levelBonus += score * player->timeBonusMultiplier;

    printf("Score: %" PRIu64 "\n", player->score);
    printf("Level Bonus: %" PRIu64 "\n", player->levelBonus);
    printf("Time Bonus Multiplier: %d\n", player->timeBonusMultiplier);
}

void initPlayer(Player* player) {
    Powerups initPowerups = {false, false, false, false, 1};
    
    player->bombs = 2;
    player->level = 1;
    player->levelBonus = 2000;
    player->lives = 2;
    player->powerups = initPowerups;
    player->score = 0;
    player->shieldPower = 50;
    player->timeBonusMultiplier = 1;
}