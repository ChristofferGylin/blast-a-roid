#ifndef SHOWFPS_H
#define SHOWFPS_H

#include "raylib.h"

typedef struct GameContext GameContext;

static const int FPS_HEADING_FONT_SIZE = 60;
static const int FPS_LINE_THICKNESS = 2;
static const int FPS_TITLE_FONT_SIZE = 18;
static const int FPS_TITLE_FONT_SPACING = 8;
static const int FPS_VALUE_FONT_SIZE = 72;
static const int FPS_GAP = 8;
static const int FRAME_TIME_VALUE_FONT_SIZE = 12;

typedef struct RenderFpsPositions {
    Vector2 currentTitle;
    Vector2 heading;
    Vector2 highestTitle;
    Vector2 lowestTitle;
    Vector2 currentValue;
    Vector2 highestValue;
    Vector2 lowestValue;
}RenderFpsPositions;

typedef struct RenderFpsTitles {
    char current[8];
    char heading[4];
    char highest[8];
    char lowest[7];
}RenderFpsTitles;

typedef struct RenderFps {
    RenderFpsPositions positions;
    RenderFpsTitles titles;
    Rectangle line;
}RenderFps;

typedef struct FrameTimePositions {
    Vector2 currentTitle;
    Vector2 currentValue;
    Vector2 heading1;
    Vector2 heading2;
    Vector2 highestTitle;
    Vector2 highestValue;
    Vector2 lowestTitle;
    Vector2 lowestValue;
}FrameTimePositions;

typedef struct FrameTimeTitles {
    char current[8];
    char heading1[6];
    char heading2[5];
    char highest[8];
    char lowest[7];
}FrameTimeTitles;


typedef struct FrameTimeValues {
    float current;
    float highest;
    float lowest;
}FrameTimeValues;

typedef struct FrameTime {
    FrameTimePositions positions;
    FrameTimeTitles titles;
    FrameTimeValues values;
    Rectangle line1;
    Rectangle line2;
}FrameTime;

typedef struct Fps {
    int currentFps;
    int lowestFps;
    int highestFps;
    RenderFps render;
    FrameTime frameTime;
}Fps;

void drawFps(GameContext* ctx, Fps* fps);
void drawFrameTime(GameContext* ctx, FrameTime* frameTime);
void initFps(Fps* fps);
void initFrameTime(FrameTime* frameTime);
void updateFps(GameContext* ctx, Fps* fps);

#endif