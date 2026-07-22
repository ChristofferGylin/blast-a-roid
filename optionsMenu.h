#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "ui.h"

#define NUMBER_OF_OPTIONS_TABS 3

typedef void (*DrawOptionsTab)(void* userData);

typedef struct AudioTabData {
    
}AudioTabData;

typedef struct ControlsTabData {
    
}ControlsTabData;

typedef struct VideoTabData {
    
}VideoTabData;

typedef struct OptionsMenuTab {
    char heading[TITLE_MAX_LENGTH];
    DrawOptionsTab drawTab;
    void *userData;
}OptionsMenuTab;

typedef struct OptionsMenu {
    BasicLayoutContainer layout;
    Button backButton;
    Button prevTabButton;
    Button nextTabButton;
    OnClickIncreaseArgs onClickIncreaseArgs;
    LayoutSection tabs[NUMBER_OF_OPTIONS_TABS];
    AudioTabData audioTabData;
    ControlsTabData controlsTabData;
    VideoTabData videoTabData;
    int selectecTab;
    bool exit;
}OptionsMenu;

bool optionsMenu(GameContext* ctx);

#endif