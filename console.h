#ifndef CONSOLE_H
#define CONSOLE_H

//Own libraries
#include "main.h"

void printInfo(Info *info);
void printData(Info data[], int length);
void printStat(char *name, Stat stat);
void printInfoStat(InfoStat *infoStat);

#endif