#include "console.h"

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
    printf("%d;", data.soilHum);
    printf("%d;\n", data.lightVal);
}