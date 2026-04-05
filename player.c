#include "player.h"
#include "asteroid.h"

void addScore(Player* player, Asteroid* ast) {
    int score = ast->level == 1 ? 50 : ast->level == 2 ? 100 : 300;
    player->score += score;
    player->levelBonus += score * player->timeBonusMultiplier;
}