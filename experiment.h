#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "main.h"
#include "gpio.h"
#include "display.h"
#include "console.h"
#include "error_led.h"
#include "graph.h"

enum experiment {
    EXP_5SEC,   // 0
    EXP_1MIN,   // 1
    EXP_5MIN,   // 2
    EXP_30MIN,  // 3
    EXP_1HOUR,  // 4
    EXP_FOREVER, // 5
};

void initDisplayExp();
void experimentSelect();
int periodicRead(Info *exp, int time);
void foreverRead();
void experimentResultsSelect(Info *data, int size);
void displayMenuExperiment(int select);
void displayForever(Info *info, int expProg);
void displayExperiment(Info *info, int expProg, int expTime);
Info getAvg(Info data[], int size);
Info getMin(Info data[], int size);
Info getMax(Info data[], int size);
void displayExperimentAvg(Info *avg);
void displayExperimentMin(Info *min);
void displayExperimentMax(Info *max);

#endif