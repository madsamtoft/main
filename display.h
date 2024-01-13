#ifndef DISPLAY_H
#define DISPLAY_H

//Own libraries
#include "config.h"

void initDisplay();
void clearScreen();
void displayMenu(int select);
void displayMenuAverage(int select);
void displayInfo(Info *info);
void displayAverage(Info *info);
void averageSelect();
void menuSelect();

#endif