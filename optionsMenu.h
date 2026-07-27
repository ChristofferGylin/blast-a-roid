#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "ui.h"

#define NUMBER_OF_OPTIONS_TABS 3
#define NUMBER_OF_VIDEO_OPTIONS 2

typedef void (*Callback)(void* userData);

typedef struct AudioTabData {
    
}AudioTabData;

typedef struct ControlsTabData {
    
}ControlsTabData;

typedef struct VideoTabData {
    CheckboxWithTitle checkboxes[NUMBER_OF_VIDEO_OPTIONS];
    char warningText[48];
    Vector2 warningTextPosition;
    bool isWarningTextVisible;
    GameContext* ctx;
}VideoTabData;

typedef struct OptionsMenuTab {
    LayoutSection layout;
    Callback updateTab;
}OptionsMenuTab;

typedef struct OptionsMenu {
    BasicLayoutContainer layout;
    Button backButton;
    Button prevTabButton;
    Button nextTabButton;
    OnClickIncreaseArgs onClickIncreaseArgs;
    OptionsMenuTab tabs[NUMBER_OF_OPTIONS_TABS];
    AudioTabData audioTabData;
    ControlsTabData controlsTabData;
    VideoTabData videoTabData;
    int selectecTab;
    bool exit;
}OptionsMenu;

bool optionsMenu(GameContext* ctx);

#endif