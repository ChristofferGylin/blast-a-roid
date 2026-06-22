#include "raylib.h"
#include "constants.h"
#include "colors.h"
#include "bonuses.h"
#include "sidebars.h"
#include "player.h"
#include "utils.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include "gameContext.h"
#include "specials.h"

void renderSidebarLeft(GameContext* ctx);
void renderSidebarRight(GameContext* ctx);

void renderLives(GameContext* ctx, Vector2 position, Vector2 size) {
    
    int gap = 4;
    int maxIcons = (size.x + gap) / (size.y + gap);
    int currentPosX = position.x;

    for (int i = 0; i < ctx->player.lives; i++) {
        if (i + 1 >= maxIcons) {

            if (i + 1 >= ctx->player.lives) {
                DrawTexturePro(
                    ctx->assets.sprites.ship,
                    (Rectangle){0, 0, ctx->assets.sprites.ship.width, ctx->assets.sprites.ship.height},
                    (Rectangle){currentPosX, position.y, size.y, size.y},
                    (Vector2){ 0, 0},
                    0,
                    primaryColor
                );
            } else {
                DrawText("+", currentPosX, position.y, 18, primaryColor);
            }
            break;
        }

        DrawTexturePro(
            ctx->assets.sprites.ship,
            (Rectangle){0, 0, ctx->assets.sprites.ship.width, ctx->assets.sprites.ship.height},
            (Rectangle){currentPosX, position.y, size.y, size.y},
            (Vector2){ 0, 0},
            0,
            primaryColor
        );

        currentPosX += size.y + gap;
    }
}

void renderPowerupIcon(Texture2D* iconSprite, Vector2 position, Vector2 size, bool active) {
    
    Texture2D sprite = *iconSprite;
    Color color = active ? primaryColor : primaryColorDimmed;
    
    DrawTexturePro(
            sprite,
            (Rectangle){0, 0, sprite.width, sprite.height},
            (Rectangle){position.x, position.y, size.y, size.y},
            (Vector2){ 0, 0},
            0,
            color
    );
}

void renderPowerups(GameContext* ctx, Vector2 position, Vector2 size) {
    
    int gap = 4;
    int numberOfIcons = 5;
    int iconsSize = (numberOfIcons * (size.y + gap)) - gap;
    int offset = (size.x - iconsSize) / 2;
    int currentPosX = position.x + offset;
    
    renderPowerupIcon(
        &ctx->assets.sprites.autoShotIcon,
        (Vector2){currentPosX, position.y},
        size,
        ctx->player.powerups.fullAuto
    );

    currentPosX += size.y + gap;

    renderPowerupIcon(
        &ctx->assets.sprites.longShotIcon,
        (Vector2){currentPosX, position.y},
        size,
        ctx->player.powerups.longShot
    );

    currentPosX += size.y + gap;
    
    renderPowerupIcon(
        &ctx->assets.sprites.triShotIcon,
        (Vector2){currentPosX, position.y},
        size,
        ctx->player.powerups.trippleShot
    );

    currentPosX += size.y + gap;

    renderPowerupIcon(
        &ctx->assets.sprites.stopIcon,
        (Vector2){currentPosX, position.y},
        size,
        ctx->player.powerups.autoStop
    );

    currentPosX += size.y + gap;

    renderPowerupIcon(
        &ctx->assets.sprites.lockIcon,
        (Vector2){currentPosX, position.y},
        size,
        ctx->player.powerups.lock
    );
}

void renderShieldPower(float shieldPower, Vector2 position, Vector2 size) {

    int barLevelWidth = size.x * shieldPower;
    float roundnessRadius = 8.0f;
    int segments = 10;

    Rectangle rect = {
        position.x,
        position.y,
        size.x,
        size.y
    };

    float roundness = getRoundness(rect, roundnessRadius);

    DrawRectangleRounded(rect, roundness, segments, primaryColorDimmed);
    BeginScissorMode(rect.x, rect.y, barLevelWidth, rect.height);
        DrawRectangleRounded(rect, roundness, segments, primaryColor);
    EndScissorMode();
}

RenderPositions renderBlock(char text[], int startY, bool isLeftSide) {
    
    int margin = 5;
    int padding = 6;
    float lineThickness = 2;
    int fontSize = 18;
    int fontSpacing = 8;
    int underlineGap = 2;
    int contentHeight = 20;

    int zeroX = isLeftSide ? 0 : SCREEN_WIDTH - SIDEBAR_WIDTH;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, fontSpacing);

    Rectangle container = {
        zeroX + margin,
        startY + margin + lineThickness,
        SIDEBAR_WIDTH - (margin * 2),
        (padding * 4) + textSize.y + contentHeight
    };

    Vector2 textOrigin = {textSize.x / 2, 0};
    Vector2 textPosition = {zeroX + (SIDEBAR_WIDTH / 2), startY + margin + lineThickness + padding};

    float roundnessRadius = 8.0f;
    int segments = 10;

    float roundness = getRoundness(container, roundnessRadius);

    Vector2 lineStart = {container.x, textPosition.y + textSize.y + padding};
    Vector2 lineEnd = {container.x + container.width, textPosition.y + textSize.y + padding};

    DrawRectangleRoundedLinesEx(container, roundness, segments, lineThickness, primaryColor);
    DrawLineEx(lineStart, lineEnd, lineThickness, primaryColor);
    DrawTextPro(GetFontDefault(), text, textPosition, textOrigin, 0, fontSize, fontSpacing, primaryColor);
    
    Vector2 contentPosition = {
        container.x + padding,
        lineStart.y + lineThickness + padding
    };

    Vector2 contentSize = {
        container.width - (padding * 2),
        contentHeight
    };

    int endYPosition = container.y + container.height;

    return (RenderPositions){contentPosition, contentSize, endYPosition};
}


void renderStats(uint64_t value, Vector2 position, Vector2 size) {

    const int fontSize = 18;
    const int fontSpacing = 8;
    const int padding = 6;
    char valueText[32];

    snprintf(valueText, sizeof(valueText), "%" PRIu64, value);

    Vector2 valueSize = MeasureTextEx(GetFontDefault(), valueText, fontSize, fontSpacing);

    Vector2 valuePosition = {
        position.x + size.x - valueSize.x,
        position.y 
    };

    Vector2 valueOrigin = {0, 0};

    DrawTextPro(GetFontDefault(), valueText, valuePosition, valueOrigin, 0, fontSize, fontSpacing, primaryColor);
}

void renderSidebarLeft(GameContext* ctx) {
    int startY = 20;
    int statsBlockGap = 25;
    int padding = 4;

    DrawRectangleGradientV(0, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, topColor, bottomColor);
    DrawLine(SIDEBAR_WIDTH, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, lineColor);

    RenderPositions scorePosition = renderBlock("SCORE", 0, true);
    renderStats(ctx->player.score, scorePosition.contentPosition, scorePosition.contentSize);

    RenderPositions bonusPosition = renderBlock("BONUS", scorePosition.endYPosition, true);
    renderStats(ctx->player.levelBonus, bonusPosition.contentPosition, bonusPosition.contentSize);
    
    RenderPositions levelPosition = renderBlock("LEVEL", bonusPosition.endYPosition, true);
    renderStats((uint64_t)ctx->player.level, levelPosition.contentPosition, levelPosition.contentSize);

    if (ctx->player.powerups.levelBonusMultiplier > 1) {
        Vector2 multiplierIconPosition = {
            scorePosition.contentPosition.x + scorePosition.contentSize.x - (padding * 2),
            scorePosition.endYPosition + padding + (BONUS_MULTIPLIER_RADIUS * 2)
        };

        renderMultiplierIcon(ctx, ctx->player.powerups.levelBonusMultiplier, multiplierIconPosition);
    }
}

void renderSidebarRight(GameContext* ctx) {
    int startY = 20;
    int statsBlockGap = 25;
    
    DrawRectangleGradientV(SCREEN_WIDTH - SIDEBAR_WIDTH, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, topColor, bottomColor);
    DrawLine(SCREEN_WIDTH - SIDEBAR_WIDTH, 0, SCREEN_WIDTH - SIDEBAR_WIDTH, SCREEN_HEIGHT, lineColor);

    RenderPositions shieldPowerPosition = renderBlock("SHIELD", 0, false);
    renderShieldPower(ctx->player.shieldPower, shieldPowerPosition.contentPosition, shieldPowerPosition.contentSize);

    RenderPositions powerupsPosition = renderBlock("POWERUPS", shieldPowerPosition.endYPosition, false);
    renderPowerups(ctx, powerupsPosition.contentPosition, powerupsPosition.contentSize);
    
    RenderPositions livesPosition = renderBlock("LIVES", powerupsPosition.endYPosition, false);
    renderLives(ctx, livesPosition.contentPosition, livesPosition.contentSize);
}

void renderSidebars(GameContext* ctx) {
    renderSidebarLeft(ctx);
    renderSidebarRight(ctx);
}
