#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "ui.h"

#define NUMBER_OF_OPTIONS_TABS 3

typedef void (*DrawOptionsTab)(void* userData);

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
    LayoutSection tabs[NUMBER_OF_OPTIONS_TABS];
    int selectecTab;
    bool exit;
}OptionsMenu;

#endif