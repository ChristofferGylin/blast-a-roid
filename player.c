#include "player.h"
#include "asteroid.h"
#include "constants.h"
#include <stdio.h>
#include <inttypes.h>
#include "raylib.h"

void initPlayer(Player* player) {
    Powerups initPowerups = {false, false, false, false, 1};
    
    player->bombs = 2;
    player->level = 1;
    player->levelBonus = 2000;
    player->levelBonusTimer = GetTime();
    player->lives = 2;
    player->powerups = initPowerups;
    player->score = 0;
    player->shieldPower = 50;
    player->timeBonusMultiplier = 1;
    player->timeBonusTimer = 0.0;
}