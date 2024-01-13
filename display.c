#include "display.h"
#include "gpio.h"
#include "buzzer.h"
#include "config.h"

void initDisplay() {
    i2c_master_shared_i2c_init(&dev);
    ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);
}

void clearScreen() { // Method to clear the screen
    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);
}

void displayMenu(int select) { // Method to display the menu
    ssd1306_display_text(&dev, 1, "Menu:", 5, false);
    ssd1306_display_text(&dev, 2, "Overview", 8, (select == 0));
    ssd1306_display_text(&dev, 3, "Run Experiment", 14, (select == 1));
    ssd1306_display_text(&dev, 4, "Soil Sensor", 11, (select == 2));
    ssd1306_display_text(&dev, 5, "Air Sensor", 10, (select == 3));
    ssd1306_display_text(&dev, 6, "Light Sensor", 12, (select == 4));
}

void displayMenuExperiment(int select) {
    ssd1306_display_text(&dev, 1, "Run exp. for:", 13, false);
    ssd1306_display_text(&dev, 2, " * 5 seconds", 12, (select == 0));
    ssd1306_display_text(&dev, 3, " * 1 minute", 11, (select == 1));
    ssd1306_display_text(&dev, 4, " * 5 minutes", 12, (select == 2));
    ssd1306_display_text(&dev, 5, " * 1 hour", 9, (select == 3));
}

void displayInfo(Info *info) { // Method to display current info values
    bool airTempError = info -> airTmp > LOW_AIR_TMP && info -> airTmp < HIGH_AIR_TMP;
    bool lightError = info -> lightVal > LOW_LIGHT;

    char airTemp[17];
    char soilTemp[17];
    char airHumidity[17];
    char soilHumidity[17];
    char lightLevel[17];
    sprintf(airTemp,        "Air  tmp: %5.1fC", info -> airTmp);
    sprintf(soilTemp,       "Soil tmp: %5.1fC", info -> soilTmp);
    sprintf(airHumidity,    "Air  hum: %5.1f%%", info -> airHum);
    sprintf(soilHumidity,   "Soil hum: %6d", info -> soilHum);
    sprintf(lightLevel,     "Lght lvl: %6d", info -> lightVal);
    ssd1306_display_text(&dev, 1, "Overview:", 9, false);
    ssd1306_display_text(&dev, 2, airTemp, 16, !airTempError);
    ssd1306_display_text(&dev, 3, soilTemp, 16, false);
    ssd1306_display_text(&dev, 4, airHumidity, 16, false);
    ssd1306_display_text(&dev, 5, soilHumidity, 16, false);
    ssd1306_display_text(&dev, 6, lightLevel, 16, !lightError);
}

void displayAverage(Info *info) {

}

void displaySoilInfo(Info *info) {
    char soilTemp[17];
    char soilHumidity[17];
    sprintf(soilTemp,       "Soil tmp: %5.1fC", info -> soilTmp);
    sprintf(soilHumidity,   "Soil hum: %6d", info -> soilHum);
    ssd1306_display_text(&dev, 1, "Soil info:", 10, false);
    ssd1306_display_text(&dev, 2, soilTemp, 16, false);
    ssd1306_display_text(&dev, 3, soilHumidity, 16, false);
}

void displayAirInfo(Info *info) {
    char airTemp[17];
    char airHumidity[17];
    sprintf(airTemp,        "Air  tmp: %5.1fC", info -> airTmp);
    sprintf(airHumidity,    "Air  hum: %5.1f%%", info -> airHum);
    ssd1306_display_text(&dev, 1, "Air info:", 9, false);
    ssd1306_display_text(&dev, 2, airTemp, 16, false);
    ssd1306_display_text(&dev, 3, airHumidity, 16, false);
}

void displayLightInfo(Info *info) {
    char lightLevel[17];
    sprintf(lightLevel,     "Lght lvl: %6d", info -> lightVal);
    ssd1306_display_text(&dev, 1, "Light info:", 11, false);
    ssd1306_display_text(&dev, 2, lightLevel, 16, false);
}


void experimentSelect() {
    int select = 0;
    while (1) {
        displayMenuExperiment(select);
        if (getEnt()) {
            resetBtns();
            clearScreen(dev);
            switch (select) {
            case EXP_5SEC:
                periodicRead(5);
                break;
            case EXP_1MIN:
                periodicRead(60);
                break;
            case EXP_5MIN:
                periodicRead(300);
                break;
            case EXP_1HOUR:
                periodicRead(6000);
                break;
            }
            currentDisplay = OVERVIEW;
            return;
        }
        if (getSel()) {
            resetBtns();
            select++;
            select %= 4;
            vTaskDelay(DELAY(100));
        }
    }
}

void menuSelect() {
    int select = OVERVIEW;
    while (1) {
        displayMenu(select);
        if (getEnt()) {
            resetBtns();
            clearScreen();
            currentDisplay = select;
            return;
        }
        if (getSel()) {
            resetBtns();
            select++;
            select %= 5;
            vTaskDelay(DELAY(100));
        }
    }
}

void displayScreen(Info *info) {
    switch (currentDisplay) {
    case OVERVIEW:
        displayInfo(info);
        break;
    case EXPERIMENT_MENU:
        experimentSelect();
        break;
    case SOIL_SENSOR:
        displaySoilInfo(info);
        break;
    case AIR_SENSOR:
        displayAirInfo(info);
        break;
    case LIGHT_SENSOR:
        displayLightInfo(info);
        break;
    default:
        displayInfo(info);
        break;
    }
}