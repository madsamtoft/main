//Own libraries
#include "config.h"
#include "air_sensor.h"
#include "soil_sensor.h"
#include "light_sensor.h"
#include "display.h"
#include "console.h"
#include "buzzer.h"
#include "gpio.h"

void app_main(void) {
    i2cConfig();
    initSoil();
    initLight();
    initButtons();
    initLEDs();
    initBuzzer();

    Info current;
    initDisplay();

    //Boot melody
    xTaskCreate(melody_load, "melody_load", 1000, NULL, 1, NULL);

    while (1) {
        if (gpio_get_level(GPIO_BTN_ENTER) == 0) {
            // btnPressed = 0;
            clearScreen();
            menuSelect();
        }
        updateInfo(&current);
        printInfo(&current);
        displayScreen(&current);
        vTaskDelay(DELAY(735)); // The time it takes to execute one iteration is 265ms
    }
}