#ifndef DISPLAY_H
#define DISPLAY_H

//Own libraries
#include "main.h"

//Threshhold
#define LOW_SOIL_HUM 670
#define HIGH_SOIL_HUM 900

#define LOW_SOIL_TMP 10
#define HIGH_SOIL_TMP 30

#define LOW_AIR_HUM 20 // Might not be needed
#define HIGH_AIR_HUM 40

#define LOW_AIR_TMP 10
#define HIGH_AIR_TMP 30

#define LOW_LIGHT 900

enum screen {
    OVERVIEW,           // 0
    EXPERIMENT_MENU,    // 1
    SOIL_SENSOR,        // 2
    AIR_SENSOR,         // 3
    LIGHT_SENSOR,       // 4
};
enum experiment {
    EXP_5SEC,   // 0
    EXP_1MIN,   // 1
    EXP_5MIN,   // 2
    EXP_30MIN,  // 3
    EXP_1HOUR   // 4

};

static char currentDisplay = OVERVIEW;
static bool airTmpError = false;
static bool soilTmpError = false;
static bool airHumError = false;
static bool soilHumError = false;
static bool lightError = false;
static bool blink = false;

void initDisplay();
void clearScreen();
void displayMenu(int select);
void displayMenuExperiment(int select);
void displayInfo(Info *info);
void displayExperiment(Info *info, int expProg, int expTime);
void displayExperimentAverage(Info *avg);
void displayExperimentMin(Info *min);
void displayExperimentMax(Info *max);
void displaySoilInfo(Info *info);
void displayAirInfo(Info *info);
void displayLightInfo(Info *info);
void displayExit();
int exitSelect();
void experimentSelect();
Info getAvg(Info data[], int size);
Info getMin(Info data[], int size);
Info getMax(Info data[], int size);
void experimentResultsSelect(Info *data, int size);
void menuSelect();
void displayScreen(Info *info);

#endif