//Own libraries
#include "config.h"
#include "air_sensor.h"
#include "soil_sensor.h"
#include "light_sensor.h"
#include "display.h"
#include "console.h"
#include "gpio.h"

void updateInfo(Info *info) {
    updateAir(info);
    updateSoil(info);
    updateLight(info);
}

void periodicRead(int time) { // Read and get average over a period of time
    printf("Reading data for %d seconds:\n", time);
    Info *data = (Info *) malloc(sizeof(Info) * time);
    for (int i = 0; i < time; i++) {
        updateInfo(&(data[i]));
        printInfo(&(data[i]));
        displayInfo(&(data[i]));
        vTaskDelay(DELAY(735)); // The time it takes to execute one iteration is 265ms so this is 1 read per second
    }
    printData(data, time);
    free(data); // Maybe needs to be moved if we want to use the array more
}

void app_main(void) {
    i2cConfig();
    initSoil();
    initLight();
    buttonConfig();

    Info current;
    initDisplay();

    while (1) {
        if (gpio_get_level(GPIO_BTN_ENTER) == 0) {
            clearScreen();
            menuSelect();
        }
        updateInfo(&current);
        printInfo(&current);
        displayInfo(&current);
        vTaskDelay(DELAY(735)); // The time it takes to execute one iteration is 265ms
    }
}