#ifndef DISPLAY_H
#define DISPLAY_H

//Own libraries
#include "main.h"
#include "gpio.h"
#include "buzzer.h"
#include "error_led.h"
#include "experiment.h"

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
    SOIL_SENSOR,        // 1
    AIR_SENSOR,         // 2
    LIGHT_SENSOR,       // 3
    EXPERIMENT_MENU,    // 4
    MENU,               // 5
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
void updateCurrentDisplay(char display);

void displayMenu(int select);
void displayInfo(Info *info);
void displaySoilInfo(Info *info);
void displayAirInfo(Info *info);
void displayLightInfo(Info *info);
void displayExit();
int exitSelect();

void menuSelect();
int displayScreen(Info *info);

#endif