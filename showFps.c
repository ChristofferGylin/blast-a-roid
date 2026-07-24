#include <stdio.h>
#include <string.h>

#include "colors.h"
#include "constants.h"
#include "gameContext.h"
#include "showFps.h"

void drawFps(GameContext* ctx, Fps* fps) {

    if (!ctx->options.video.showFps) return;

    char currentValue[4] = "-";
    char highestValue[4] = "-";
    char lowestValue[4] = "-";

    if (fps->currentFps > 0) {
        snprintf(currentValue, sizeof(currentValue), "%d", fps->currentFps);
    }

    if (fps->highestFps > 0) {
        snprintf(highestValue, sizeof(highestValue), "%d", fps->highestFps);
    }

    if (fps->lowestFps > 0) {
        snprintf(lowestValue, sizeof(lowestValue), "%d", fps->lowestFps);
    }

    Vector2 currentValueSize = MeasureTextEx(GetFontDefault(), currentValue, FPS_VALUE_FONT_SIZE, FPS_TITLE_FONT_SPACING);
    Vector2 highestValueSize = MeasureTextEx(GetFontDefault(), highestValue, FPS_VALUE_FONT_SIZE, FPS_TITLE_FONT_SPACING);
    Vector2 lowestValueSize = MeasureTextEx(GetFontDefault(), lowestValue, FPS_VALUE_FONT_SIZE, FPS_TITLE_FONT_SPACING);

    Vector2 currentValuePosition = fps->render.positions.currentValue;
    Vector2 highestValuePosition = fps->render.positions.highestValue;
    Vector2 lowestValuePosition = fps->render.positions.lowestValue;

    currentValuePosition.x -= currentValueSize.x;
    highestValuePosition.x -= highestValueSize.x;
    lowestValuePosition.x -= lowestValueSize.x;

    Vector2 origin = {0, 0};

    DrawTextPro(
        GetFontDefault(),
        fps->render.titles.heading,
        fps->render.positions.heading,
        origin,
        0,
        FPS_HEADING_FONT_SIZE,
        FPS_TITLE_FONT_SPACING,
        primaryColorDimmed60
    );

    DrawTextPro(
        GetFontDefault(),
        fps->render.titles.current,
        fps->render.positions.currentTitle,
        origin,
        0,
        FPS_TITLE_FONT_SIZE,
        FPS_TITLE_FONT_SPACING,
        primaryColor
    );

    DrawTextPro(
        GetFontDefault(),
        currentValue,
        currentValuePosition,
        origin,
        0,
        FPS_VALUE_FONT_SIZE,
        FPS_TITLE_FONT_SPACING,
        primaryColor
    );

    DrawTextPro(
        GetFontDefault(),
        fps->render.titles.highest,
        fps->render.positions.highestTitle,
        origin,
        0,
        FPS_TITLE_FONT_SIZE,
        FPS_TITLE_FONT_SPACING,
        primaryColor
    );

    DrawTextPro(
        GetFontDefault(),
        highestValue,
        highestValuePosition,
        origin,
        0,
        FPS_VALUE_FONT_SIZE,
        FPS_TITLE_FONT_SPACING,
        primaryColor
    );
    
    DrawTextPro(
        GetFontDefault(),
        fps->render.titles.lowest,
        fps->render.positions.lowestTitle,
        origin,
        0,
        FPS_TITLE_FONT_SIZE,
        FPS_TITLE_FONT_SPACING,
        primaryColor
    );

    DrawTextPro(
        GetFontDefault(),
        lowestValue,
        lowestValuePosition,
        origin,
        0,
        FPS_VALUE_FONT_SIZE,
        FPS_TITLE_FONT_SPACING,
        primaryColor
    );

    DrawRectanglePro(fps->render.line, origin, 0, primaryColorDimmed60);
}

void initFps(Fps* fps) {

    strcpy(fps->render.titles.current, "CURRENT");
    strcpy(fps->render.titles.highest, "HIGHEST");
    strcpy(fps->render.titles.lowest, "LOWEST");
    strcpy(fps->render.titles.heading, "FPS");

    char value[] = "999";
    
    fps->currentFps = 0;
    fps->highestFps = 0;
    fps->lowestFps = 0;

    Vector2 currentTitleSize = MeasureTextEx(GetFontDefault(), fps->render.titles.current, FPS_TITLE_FONT_SIZE, FPS_TITLE_FONT_SPACING);
    Vector2 headingSize = MeasureTextEx(GetFontDefault(), fps->render.titles.heading, FPS_HEADING_FONT_SIZE, FPS_TITLE_FONT_SPACING);
    Vector2 highestTitleSize = MeasureTextEx(GetFontDefault(), fps->render.titles.highest, FPS_TITLE_FONT_SIZE, FPS_TITLE_FONT_SPACING);
    Vector2 lowestTitleSize = MeasureTextEx(GetFontDefault(), fps->render.titles.lowest, FPS_TITLE_FONT_SIZE, FPS_TITLE_FONT_SPACING);
    
    Vector2 valueSize = MeasureTextEx(GetFontDefault(), value, FPS_VALUE_FONT_SIZE, FPS_TITLE_FONT_SPACING);

    float totalHeight =
        headingSize.y +
        FPS_LINE_THICKNESS +
        currentTitleSize.y +
        highestTitleSize.y +
        lowestTitleSize.y +
        (valueSize.y * 3) + 
        (FPS_GAP * 12); 

    float center = (SIDEBAR_WIDTH / 2.0f);

    fps->render.positions.heading = (Vector2){
        center - (headingSize.x / 2.0f),
        SCREEN_HEIGHT - totalHeight
    };

    fps->render.line = (Rectangle){
        fps->render.positions.heading.x,
        fps->render.positions.heading.y + headingSize.y,
        headingSize.x,
        FPS_LINE_THICKNESS
    };

    fps->render.positions.currentTitle = (Vector2){
        center - (currentTitleSize.x / 2.0f),
        fps->render.line.y + fps->render.line.height + (FPS_GAP * 3)
    };

    fps->render.positions.currentValue = (Vector2){
        center + (valueSize.x / 2),
        fps->render.positions.currentTitle.y + currentTitleSize.y + FPS_GAP
    };

    fps->render.positions.highestTitle = (Vector2){
        center - (highestTitleSize.x / 2.0f),
        fps->render.positions.currentValue.y + valueSize.y + (FPS_GAP * 2)
    };

    fps->render.positions.highestValue = (Vector2){
        center + (valueSize.x / 2),
        fps->render.positions.highestTitle.y + highestTitleSize.y + FPS_GAP
    };

    fps->render.positions.lowestTitle = (Vector2){
        center - (lowestTitleSize.x / 2.0f),
        fps->render.positions.highestValue.y + valueSize.y + (FPS_GAP * 2)
    };

    fps->render.positions.lowestValue = (Vector2){
        center + (valueSize.x / 2),
        fps->render.positions.lowestTitle.y + lowestTitleSize.y + FPS_GAP
    };
}

void updateFps(GameContext* ctx, Fps* fps) {

    if (!ctx->options.video.showFps) return;

    fps->currentFps = GetFPS();

    if (fps->currentFps > fps->highestFps) {
        fps->highestFps = fps->currentFps;
    }

    if (fps->lowestFps == 0 || fps->currentFps < fps->lowestFps) {
        fps->lowestFps = fps->currentFps;
    }
}