#include "console.h"


void printInfo(Info *info) {
    // Print info
    printf("Air Temperature: %.1f°C, Air Humidity: %.1f%%\n", info -> airTmp, info -> airHum);
    printf("Soil Temperature: %.1f°C, Soil Moisture: %u\n", info -> soilTmp, info -> soilHum);
    printf("Light value: %d\n", info -> lightVal);
    printf("\n");
}

void printData(Info data[], int length) {
    printf("airTMP\tairHUM\tsoilTMP\tsoilHUM\tlight\n");
    float airTmpSum = 0;
    float airHumSum = 0;
    float soilTmpSum = 0;
    float soilHumSum = 0;
    float lightValSum = 0;
    for (int i = 0; i < length; i++) {
        printf("%.1f\t", data[i].airTmp);
        printf("%.1f\t", data[i].airHum);
        printf("%.1f\t", data[i].soilTmp);
        printf("%d\t", data[i].soilHum);
        printf("%d\t\n", data[i].lightVal);
        airTmpSum += data[i].airTmp;
        airHumSum += data[i].airHum;
        soilTmpSum += data[i].soilTmp;
        soilHumSum += data[i].soilHum;
        lightValSum += data[i].lightVal;
    }
    printf("\nAverage readings:\n");
    printf("%.1f\t", airTmpSum / length);
    printf("%.1f\t", airHumSum / length);
    printf("%.1f\t", soilTmpSum / length);
    printf("%.1f\t", soilHumSum / length);
    printf("%.1f\t\n", lightValSum / length);
}