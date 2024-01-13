#ifndef DISPLAY_H
#define DISPLAY_H

//Own libraries
#include "config.h"

enum screen {
    OVERVIEW,       // 0
    AVERAGE_MENU,   // 1
    SOIL_SENSOR,    // 2
    AIR_SENSOR,     // 3
    LIGHT_SENSOR,   // 4
};
enum averages {
    AVERAGE_5SEC,    // 0
    AVERAGE_1MIN,   // 1
    AVERAGE_5MIN,   // 2
    AVERAGE_1HOUR  // 3
};

static char currentDisplay = OVERVIEW;

void initDisplay();
void clearScreen();
void displayMenu(int select);
void displayMenuAverage(int select);
void displayInfo(Info *info);
void displayAverage(Info *info);
void displaySoilInfo(Info *info);
void displayAirInfo(Info *info);
void displayLightInfo(Info *info);
void averageSelect();
void menuSelect();
void displayScreen(Info *info);


#endif