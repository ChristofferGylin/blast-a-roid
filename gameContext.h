#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include "bonuses.h"
#include "debug.h"
#include "raylib.h"
#include "asteroid.h"
#include "enemies.h"
#include "shooting.h"
#include "animation.h"
#include "options.h"
#include "player.h"
#include "ship.h"
#include "specials.h"
#include "score.h"

static const int FPS_TITLE_FONT_SIZE = 18;
static const int FPS_TITLE_FONT_SPACING = 8;
static const int FPS_CURRENT_VALUE_FONT_SIZE = 24;
static const int FPS_VALUE_FONT_SIZE = 12;
static const int FPS_GAP = 8;

typedef struct Animations {
    Animation blackHole;
    Animation comet;
    Animation crate;
    Animation explosion;
    Animation metalAsteroid;
    Animation multiplier;
    Animation spikyAsteroid;
    Animation supernova;
    Animation ufo1;
    Animation ufo2;
    Animation ufo3;
}Animations;

typedef struct Samples {
    Sound alarm;
    Sound explosion;
    Sound metalPlink;
    Sound multiplier_collect;
    Sound multiplier_spawn;
    Sound shieldUp;
    Sound shot;
    Sound supernova;
    Sound enemyShot1;
}Samples;

typedef struct Sprites {
    Texture2D asteroid;
    Texture2D autoShotIcon;
    Texture2D destroyedShip1;
    Texture2D destroyedShip2;
    Texture2D destroyedShip3;
    Texture2D blueGem;
    Texture2D lockIcon;
    Texture2D longShotIcon;
    Texture2D ship;
    Texture2D shot;
    Texture2D stopIcon;
    Texture2D triShotIcon;
    Texture2D enemyShot1;
}Sprites;

typedef struct Assets {
    Animations animations;
    Samples samples;
    Sprites sprites;
}Assets;

typedef struct ObjectPools {
    AsteroidPool asteroids;
    BonusObjectPool bonuses;
    BonusSpawnPool spawnableBonuses;
    DestroyedAsteroidPool destroyedAsteroids;
    EnemyObjectPool enemies;
    EnemySpawnPool spawnableEnemies;
    AnimationPool explosions;
    ShotObjectPool shots;
    SpecialsPool specials;
    SpecialsSpawnPool specialsSpawn;
}ObjectPools;

typedef struct SpawningEnemies {
    double nextSpawn;
    FloatRange spawnDelay;
}SpawningEnemies;

typedef struct Supernova {
    bool detonated;
    double detonationTime;
    double shakeTimer;
}Supernova;

typedef struct RenderFpsPositions {
    Vector2 currentTitle;
    Vector2 highestTitle;
    Vector2 lowestTitle;
    Vector2 currentValue;
    Vector2 highestValue;
    Vector2 lowestValue;
}RenderFpsPositions;

typedef struct RenderFpsTitles {
    char current[8];
    char highest[8];
    char lowest[7];
}RenderFpsTitles;

typedef struct RenderFps {
    RenderFpsPositions positions;
    RenderFpsTitles titles;
}RenderFps;

typedef struct Fps {
    int currentFps;
    int lowestFps;
    int highestFps;
    RenderFps render;
}Fps;

typedef struct GameContext {
    Assets assets;
    Debug debug;
    ObjectPools objectPools;
    Player player;
    Ship ship;
    SpawningEnemies spawning;
    double pausTimer;
    Supernova supernova;
    bool isBlackHoleActive;
    Highscores highscores;
    Options options;
    Fps fps;
}GameContext;

void initFps(Fps* fps);
void initGameContext(GameContext* ctx, bool debugActive);
void initObjectPools(GameContext* ctx);
void initSpawning(GameContext* ctx);
void loadAssets(GameContext* ctx);
void updateFps(GameContext* ctx);
void unloadAssets(GameContext* ctx);

#endif