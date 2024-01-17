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

void printInfo(Info *info) {
    // Print info
    printf("Air Temperature: %.1f°C, Air Humidity: %.1f%%\n", info -> airTmp, info -> airHum);
    printf("Soil Temperature: %.1f°C, Soil Moisture: %u\n", info -> soilTmp, info -> soilHum);
    printf("Light value: %d\n", info -> lightVal);
    printf("\n");
}

void printData(Info data[], int length) {
    printf("Time;airTMP;soilTMP;airHUM;soilHUM;light\n");
    for (int i = 0; i < length; i++) {
        printf("%d;",i);
        printf("%.1f;", data[i].airTmp);
        printf("%.1f;", data[i].airHum);
        printf("%.1f;", data[i].soilTmp);
        printf("%d;", data[i].soilHum);
        printf("%d;\n", data[i].lightVal);
    }
}