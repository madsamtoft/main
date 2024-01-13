//Own libraries
#include "config.h"
#include "air_sensor.h"
#include "soil_sensor.h"
#include "light_sensor.h"
#include "display.h"
#include "console.h"
#include "buzzer.h"


void app_main(void) {
    i2cConfig();
    initSoil();
    initLight();
    buttonConfig();
    initBuzzer();

    Info current;
    initDisplay();

    melody_load();

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