#include <stdio.h>
#include <string.h>

#include "colors.h"
#include "config.h"
#include "constants.h"
#include "drawing.h"
#include "gameContext.h"
#include "optionsMenu.h"
#include "raylib.h"
#include "ui.h"
#include "uiSizes.h"

void drawOptionsMenu(GameContext* ctx, OptionsMenu* menu);
void drawOptionsMenuTab(OptionsMenu* menu);
void initOptionsMenu(GameContext* ctx, OptionsMenu* menu);
void initOptionsMenuTab(OptionsMenuTab* tab, Rectangle* parent, char* heading, Callback drawContent, Callback updateTab, void* userData);
void initHighscoresTabData(GameContext* ctx, Rectangle* parent, HighscoresTabData* tabData);
void initVideoTabData(GameContext* ctx, Rectangle* parent, VideoTabData* tabData);
void drawVideoTab(void* userData);
void drawControlsTab(void* userData);
void drawAudioTab(void* userData);
void drawHighscoresTab(void* userData);
void setFullscreenCallback(void* userData);
void setMonitorCallback(int monitor, void* userData);
void updateOptionsMenu(GameContext* ctx, OptionsMenu* menu);
void updateOptionsMenuTab(OptionsMenu* menu);
void updateAudioTab(void* userData);
void updateControlsTab(void* userData);
void updateHighscoresTab(void* userData);
void updateVideoTab(void* userData);

void updateAudioTab(void* userData) {};
void updateControlsTab(void* userData) {};

void drawControlsTab(void* userData) {};
void drawAudioTab(void* userData) {};

void initHighscoresTabData(GameContext* ctx, Rectangle* parent, HighscoresTabData* tabData) {
    Vector2 position = {parent->x, parent->y};
    
    initButton(
        &tabData->eraseHighScoresButton,
        (Rectangle){position.x, position.y, 0, 0},
        BUTTON_FONT_SIZE,
        "Erase highscores",
        toggleBoolCallback,
        &tabData->confirmDialogBox.isVisible
    );
    
    initDialogBox(
        &tabData->confirmDialogBox,
        "Are you sure you want to erase the highscore list?",
        "Cancel",
        "Erase",
        resetHighscoresCallback,
        &ctx->highscores
    );
}

void initOptionsMenu(GameContext* ctx, OptionsMenu* menu) {
    menu->exit = false;
    menu->onClickIncreaseArgs.max_Value = NUMBER_OF_OPTIONS_TABS -1;
    menu->onClickIncreaseArgs.value = &menu->selectedTab;
    menu->selectedTab = 0;

    initBasicLayoutContainer(&menu->layout, (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, "OPTIONS");
    initButton(
        &menu->backButton,
        (Rectangle){MENU_MARGIN * 2.0f, (MENU_MARGIN * 2.0f) + MENU_LINE_THICKNESS, 0, 0},
        BUTTON_FONT_SIZE, "BACK",
        onClickBack,
        &menu->exit
    );
    
    initOptionsMenuTab(&menu->tabs[0], &menu->layout.contentArea, "VIDEO", drawVideoTab, updateVideoTab, &menu->videoTabData);
    initOptionsMenuTab(&menu->tabs[1], &menu->layout.contentArea, "AUDIO", drawAudioTab, updateAudioTab, &menu->audioTabData);
    initOptionsMenuTab(&menu->tabs[2], &menu->layout.contentArea, "CONTROLS", drawControlsTab, updateControlsTab, &menu->controlsTabData);
    initOptionsMenuTab(&menu->tabs[3], &menu->layout.contentArea, "HIGHSCORES", drawHighscoresTab, updateHighscoresTab, &menu->highscoresTabData);

    initVideoTabData(ctx, &menu->tabs[0].layout.contentArea, &menu->videoTabData);
    initHighscoresTabData(ctx, &menu->tabs[3].layout.contentArea, &menu->highscoresTabData);

    float largestHeadingSize = 0;

    for (int i = 0; i < NUMBER_OF_OPTIONS_TABS; i++) {
        Vector2 headingSize = MeasureTextEx(GetFontDefault(), menu->tabs[i].layout.heading, OPTIONS_TAB_HEADING_FONT_SIZE, MENU_FONT_SPACING);

        if (headingSize.x > largestHeadingSize) largestHeadingSize = headingSize.x;
    }

    Rectangle previousTabButtonRect;

    previousTabButtonRect.width = 32;
    previousTabButtonRect.height = 32;
    previousTabButtonRect.x = menu->tabs[0].layout.contentArea.x + (menu->tabs[0].layout.contentArea.width / 2.0f) - (largestHeadingSize / 2.0f) - (previousTabButtonRect.width * 2);
    previousTabButtonRect.y = menu->tabs[0].layout.headingPosition.y;

    Rectangle nextTabButtonRect;

    nextTabButtonRect.width = 32;
    nextTabButtonRect.height = 32;
    nextTabButtonRect.x = menu->tabs[0].layout.contentArea.x + (menu->tabs[0].layout.contentArea.width / 2.0f) + (largestHeadingSize / 2.0f) + nextTabButtonRect.width;
    nextTabButtonRect.y = menu->tabs[0].layout.headingPosition.y;

    initButton(&menu->nextTabButton, nextTabButtonRect, OPTIONS_TAB_HEADING_FONT_SIZE, ">", onClickIncrease, &menu->onClickIncreaseArgs);
    initButton(&menu->prevTabButton, previousTabButtonRect, OPTIONS_TAB_HEADING_FONT_SIZE, "<", onClickDecrease, &menu->onClickIncreaseArgs);

}

void initOptionsMenuTab(OptionsMenuTab* tab, Rectangle* parent, char* heading, Callback drawContent, Callback updateTab, void* userData) {

    tab->updateTab = updateTab;

    tab->layout.drawContent = drawContent;
    tab->layout.userData = userData;

    tab->layout.container.x = parent->x + MENU_MARGIN;
    tab->layout.container.y = parent->y + MENU_MARGIN;
    tab->layout.container.width = parent->width - (MENU_MARGIN * 2);
    tab->layout.container.height = parent->height - (MENU_MARGIN * 2);

    Vector2 headingSize = MeasureTextEx(GetFontDefault(), heading, OPTIONS_TAB_HEADING_FONT_SIZE, MENU_FONT_SPACING);

    tab->layout.headingPosition.x = tab->layout.container.x + (tab->layout.container.width / 2.0f) - (headingSize.x / 2.0f);
    tab->layout.headingPosition.y = tab->layout.container.y;

    tab->layout.divider.x = tab->layout.headingPosition.x;
    tab->layout.divider.y = tab->layout.headingPosition.y + headingSize.y + (MENU_MARGIN / 2.0f); 
    tab->layout.divider.width = headingSize.x;
    tab->layout.divider.height = SECTION_DIVIDER_LINE_THICKNESS;

    tab->layout.contentArea.x = tab->layout.container.x;
    tab->layout.contentArea.y = tab->layout.divider.y + tab->layout.divider.height + MENU_MARGIN;
    tab->layout.contentArea.width = tab->layout.container.width;
    tab->layout.contentArea.height = tab->layout.container.height - (tab->layout.contentArea.y - tab->layout.container.y);

    strcpy(tab->layout.heading, heading);
}

void setFullscreenCallback(void* userData) {
    Rendering* rendering = userData;
    
    ToggleBorderlessWindowed();
    initRendering(rendering);
}

void setMonitorCallback(int monitor, void* userData) {
    GameContext* ctx = userData;

    if (ctx->options.video.fullscreen) {
        setFullscreenCallback(ctx);
        SetWindowMonitor(monitor);
        setFullscreenCallback(ctx);
    } else {
        SetWindowMonitor(monitor);
    }

    ctx->options.video.selectecMonitor = monitor;
    ctx->options.video.isMonitorSetByUser = true;

    if (!ctx->options.video.vSync) {
        setUserRefreshRate();
    }
};

void initVideoTabData(GameContext* ctx, Rectangle* parent, VideoTabData* tabData) {
    Vector2 position = {parent->x, parent->y};
    int yOffset = CHECKBOX_SIZE * 2;

    int monitorCount = GetMonitorCount();
    

    DropDownTitles monitorTitles;

    for (int i = 0; i < monitorCount; i++) {
        strcpy(monitorTitles[i], GetMonitorName(i));
    }

    strcpy(tabData->monitorSelect.heading, "Selected monitor");

    tabData->monitorSelect.headingPosition = position;

    Vector2 headingSize = MeasureTextEx(
        GetFontDefault(),
        tabData->monitorSelect.heading,
        OPTIONS_MENU_FONT_SIZE,
        MENU_FONT_SPACING
    );

    position.y += headingSize.y + (CHECKBOX_SIZE / 2.0f);
    
    initDropdownMenu(
        &tabData->monitorSelect.dropdown,
        monitorTitles,
        monitorCount,
        GetCurrentMonitor(),
        (Rectangle){
            position.x,
            position.y,
            200,
            0,
        },
        setMonitorCallback,
        ctx
    );

    position.y += yOffset;

    initCheckboxWithTitle(&tabData->checkboxes[0], position, "Fullscreen", &ctx->options.video.fullscreen, setFullscreenCallback, &ctx->rendering);

    position.y += yOffset;

    initCheckboxWithTitle(&tabData->checkboxes[1], position, "Show FPS", &ctx->options.video.showFps, NULL, NULL);

    position.y += yOffset;

    initCheckboxWithTitle(&tabData->checkboxes[2], position, "V-Sync", &ctx->options.video.vSync, NULL, NULL);

    position.y += yOffset;

    tabData->warningTextPosition = position;
    strcpy(tabData->warningText, "Game restart needed for changes to take effect.");
    tabData->isWarningTextVisible = false;
    tabData->ctx = ctx;
}

void drawOptionsMenu(GameContext* ctx, OptionsMenu* menu) {
    
    BeginTextureMode(ctx->rendering.renderTexture);
        ClearBackground(BLACK);
        drawBasicLayoutContainer(&menu->layout);
        drawButton(&menu->backButton);
        drawOptionsMenuTab(menu);
    EndTextureMode();

    renderToScreen(&ctx->rendering);
}

void drawOptionsMenuTab(OptionsMenu* menu) {
    LayoutSection* section = &menu->tabs[menu->selectedTab].layout;

    Vector2 origin = {0,0};

    drawButton(&menu->nextTabButton);
    drawButton(&menu->prevTabButton);
    DrawTextPro(GetFontDefault(), section->heading, section->headingPosition, origin, 0, OPTIONS_TAB_HEADING_FONT_SIZE, MENU_FONT_SPACING, primaryColor);
    DrawRectanglePro(section->divider, origin, 0, primaryColor);

    section->drawContent(section->userData);
}

void drawHighscoresTab(void* userData) {
    HighscoresTabData* data = userData;
}

void drawVideoTab(void* userData) {
    VideoTabData* data = userData;

    for (int i = 0; i < NUMBER_OF_VIDEO_OPTIONS; i++) {
        drawCheckboxWithTitle(&data->checkboxes[i]);
    }

    if (data->isWarningTextVisible) {
        DrawTextPro(
            GetFontDefault(),
            data->warningText,
            data->warningTextPosition,
            (Vector2){0,0},
            0,
            OPTIONS_MENU_FONT_SIZE,
            MENU_FONT_SPACING,
            RAYWHITE
        );
    }

    DrawTextPro(
        GetFontDefault(),
        data->monitorSelect.heading,
        data->monitorSelect.headingPosition,
        (Vector2){0,0},
        0,
        OPTIONS_MENU_FONT_SIZE,
        MENU_HEADING_FONT_SPACING,
        primaryColor
    );

    drawDropdownMenu(&data->monitorSelect.dropdown);
}

bool optionsMenu(GameContext* ctx) {

    ShowCursor();

    Config initialConfigState = getConfig(ctx);
    
    OptionsMenu menu;

    initOptionsMenu(ctx, &menu);

    bool applicationIsRunning = true;

    while (!WindowShouldClose()) {
        updateOptionsMenu(ctx, &menu);
        drawOptionsMenu(ctx, &menu);

        if (menu.exit) break;
    }

    if (WindowShouldClose()) {
        applicationIsRunning = false;
    } else {
        Config endConfigState = getConfig(ctx);
        
        if (!compareConfig(&initialConfigState, &endConfigState)) {
            saveConfigToFile(ctx);
        }
    }

    return applicationIsRunning;
}

void updateOptionsMenu(GameContext* ctx, OptionsMenu* menu) {
    updateButton(&menu->backButton);
    updateButton(&menu->prevTabButton);
    updateButton(&menu->nextTabButton);
    updateOptionsMenuTab(menu);
    updateRendering(&ctx->rendering);
}

void updateOptionsMenuTab(OptionsMenu* menu) {
    OptionsMenuTab* tab = &menu->tabs[menu->selectedTab];

    tab->updateTab(tab->layout.userData);
}

void updateHighscoresTab(void* userData) {
    HighscoresTabData* tabData = userData;

    updateButton(&tabData->eraseHighScoresButton);
    updateDialogBox(&tabData->confirmDialogBox);
}

void updateVideoTab(void* userData) {
    VideoTabData* tabData = userData;

    updateDropdownMenu(&tabData->monitorSelect.dropdown);

    for (int i = 0; i < NUMBER_OF_VIDEO_OPTIONS; i++) {
        updateCheckbox(&tabData->checkboxes[i].checkbox);
    }

    if (tabData->ctx->isVsyncEnabled != tabData->ctx->options.video.vSync) {
        tabData->isWarningTextVisible = true;
    } else {
        tabData->isWarningTextVisible = false;
    }
}