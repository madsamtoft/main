#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

//Own libraries
#include "main.h"

void initSoil();
void updateSoil(Info *soil);
float soilPercentage(Info *soil);

#endif