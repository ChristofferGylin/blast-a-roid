#include <stdbool.h>
#include <stdio.h>

#include "debug.h"
#include "gameContext.h"

void initDebug(Debug* debug, bool active) {
    debug->active = active;
    debug->updateTimer = 0.0f;

    debug->poolCount.asteroids.activeCount = 0;
    debug->poolCount.asteroids.capacity = MAX_ASTEROIDS;
    debug->poolCount.asteroids.spike = 0;

    debug->poolCount.bonuses.activeCount = 0;
    debug->poolCount.bonuses.capacity = MAX_BONUSES;
    debug->poolCount.bonuses.spike = 0;
    
    debug->poolCount.destroyedAsteroids.activeCount = 0;
    debug->poolCount.destroyedAsteroids.capacity = MAX_ASTEROIDS;
    debug->poolCount.destroyedAsteroids.spike = 0;
    
    debug->poolCount.enemies.activeCount = 0;
    debug->poolCount.enemies.capacity = MAX_ENEMIES;
    debug->poolCount.enemies.spike = 0;
    
    debug->poolCount.explosions.activeCount = 0;
    debug->poolCount.explosions.capacity = MAX_ANIMATIONS;
    debug->poolCount.explosions.spike = 0;
    
    debug->poolCount.shots.activeCount = 0;
    debug->poolCount.shots.capacity = MAX_SHOTS;
    debug->poolCount.shots.spike = 0;
    
    debug->poolCount.spawnableBonuses.activeCount = 0;
    debug->poolCount.spawnableBonuses.capacity = NUMBER_OF_BONUS_TYPES;
    debug->poolCount.spawnableBonuses.spike = 0;
    
    debug->poolCount.spawnableEnemies.activeCount = 0;
    debug->poolCount.spawnableEnemies.capacity = NUMBER_OF_ENEMY_TYPES;
    debug->poolCount.spawnableEnemies.spike = 0;
    
    debug->poolCount.specials.activeCount = 0;
    debug->poolCount.specials.capacity = NUMBER_OF_SPECIALS;
    debug->poolCount.specials.spike = 0;
    
    debug->poolCount.specialsSpawn.activeCount = 0;
    debug->poolCount.specialsSpawn.capacity = NUMBER_OF_SPECIALS;
    debug->poolCount.specialsSpawn.spike = 0;
    
}