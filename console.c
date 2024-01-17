#include "console.h"

void updateStat(float count, float value, Stat *stat) {
    if (count == 0) {
        stat->avg = value;
        stat->min = value;
        stat->max = value;
        return;
    }
    float a = count / (count + 1);
    float b = 1.0 / (count + 1);
    stat->avg = stat->avg * a + value * b;
    stat->min = fmin(stat->min, value);
    stat->max = fmax(stat->max, value);
}

void updateInfoStat(Info *info, InfoStat *infoStat) {
    updateStat(infoStat->count, info->airTmp, &(infoStat->airTmp));
    updateStat(infoStat->count, info->airHum, &(infoStat->airHum));
    updateStat(infoStat->count, info->soilHum, &(infoStat->soilHum));
    updateStat(infoStat->count, info->soilTmp, &(infoStat->soilTmp));
    updateStat(infoStat->count, info->lightVal, &(infoStat->lightVal));
    infoStat->count ++;
}

void printStat(char *name, Stat stat) {
    printf("%s (Min;Avg;Max): %.1f;", name,stat.min);
    printf("%.1f;%.1f\n", stat.avg, stat.max);
}

void printInfoStat(InfoStat *infoStat) {
    printStat("Air Tmp", infoStat->airTmp);
    printStat("Air Hum", infoStat->airHum);
    printStat("Soil Hum", infoStat->soilHum);
    printStat("Soil Tmp", infoStat->soilTmp);
    printStat("Light Val", infoStat->lightVal);
    printf("\n");
}

void printForever(Info data, int count) {
    printf("%d;", count);
    printf("%.1f;", data.airTmp);
    printf("%.1f;", data.airHum);
    printf("%.1f;", data.soilTmp);
    printf("%.1f;", soilPercentage(&data));
    printf("%.1f;\n", lightPercentage(&data));
}