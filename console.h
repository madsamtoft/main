#ifndef CONSOLE_H
#define CONSOLE_H

//Own libraries
#include "main.h"
#include "soil_sensor.h"
#include "light_sensor.h"

typedef struct {
    float avg;
    float min;
    float max;
} Stat;

typedef struct {
    Stat airTmp;
    Stat airHum;
    Stat soilHum;
    Stat soilTmp;
    Stat lightVal;
    long count;
} InfoStat;

void updateStat(float count, float value, Stat *stat);
void updateInfoStat(Info *info, InfoStat *infoStat);
void printStat(char *name, Stat stat);
void printInfoStat(InfoStat *infoStat);
void printForever(Info data, int count);

#endif