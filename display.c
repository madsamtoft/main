#include "display.h"

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

void updateCurrentDisplay(char display) {
    currentDisplay = display;
}

void displayInfo(Info *info) { // Method to display current info values    
    airTmpError = info -> airTmp < LOW_AIR_TMP || info -> airTmp > HIGH_AIR_TMP;
    soilTmpError = info -> soilTmp < LOW_SOIL_TMP || info -> soilTmp > HIGH_SOIL_TMP;
    airHumError = info -> airHum < LOW_AIR_HUM || info -> airHum > HIGH_AIR_HUM;
    soilHumError = info -> soilHum < LOW_SOIL_HUM || info -> soilHum > HIGH_SOIL_HUM;
    lightError = info -> lightVal < LOW_LIGHT;
    blink = !blink;

    char airTemp[17];
    char soilTemp[17];
    char airHumidity[17];
    char soilHumidity[17];
    char lightLevel[17];

    sprintf(airTemp,        "Air  tmp: %5.1fC", info -> airTmp);
    sprintf(soilTemp,       "Soil tmp: %5.1fC", info -> soilTmp);
    sprintf(airHumidity,    "Air  hum: %5.1f%%", info -> airHum);
    sprintf(soilHumidity,   "Soil hum: %5.1f%%", info -> soilHum);
    sprintf(lightLevel,     "Lght lvl: %5.1f%%", info -> lightVal);

    ssd1306_display_text(&dev, 1, "Overview:", 9, false);
    ssd1306_display_text(&dev, 2, airTemp, 16, airTmpError && blink);
    ssd1306_display_text(&dev, 3, soilTemp, 16, soilTmpError && blink);
    ssd1306_display_text(&dev, 4, airHumidity, 16, airHumError && blink);
    ssd1306_display_text(&dev, 5, soilHumidity, 16, soilHumError && blink);
    ssd1306_display_text(&dev, 6, lightLevel, 16, lightError && blink);
    ssd1306_clear_line(&dev, 7, false);
}

void displaySoilInfo(Info *info) {
    bool soilTmpHigh = info -> soilTmp > HIGH_SOIL_TMP;
    bool soilTmpLow = info -> soilTmp < LOW_SOIL_TMP;
    bool soilHumHigh = info -> soilHum > HIGH_SOIL_HUM;
    bool soilHumLow = info -> soilHum < LOW_SOIL_HUM;
    
    char soilTemp[17];
    char soilHumidity[17];
    char soilTempStat[17];
    char soilHumidityStat[17];

    sprintf(soilTemp,         "Soil tmp: %5.1fC", info -> soilTmp);
    sprintf(soilHumidity,     "Soil hum: %5.1f%%", info -> soilHum);
    sprintf(soilTempStat,     "Tmp Stat: %6s", soilTmpHigh ? "HIGH" : (soilTmpLow ? "LOW" : "OK"));
    sprintf(soilHumidityStat, "Hum Stat: %6s", soilHumHigh ? "HIGH" : (soilHumLow ? "LOW" : "OK"));

    ssd1306_display_text(&dev, 1, "Soil info:", 10, false);
    ssd1306_display_text(&dev, 2, soilTemp, 16, false);
    ssd1306_display_text(&dev, 3, soilHumidity, 16, false);
    ssd1306_display_text(&dev, 5, soilTempStat, 16, false);
    ssd1306_display_text(&dev, 6, soilHumidityStat, 16, false);
}

void displayAirInfo(Info *info) {
    bool airTmpHigh = info -> airTmp > HIGH_AIR_TMP;
    bool airTmpLow = info -> airTmp < LOW_AIR_TMP;
    bool airHumHigh = info -> airHum > HIGH_AIR_HUM;
    bool airHumLow = info -> airHum < LOW_AIR_HUM;

    char airTemp[17];
    char airHumidity[17];
    char airTempStat[17];
    char airHumidityStat[17];

    sprintf(airTemp,        "Air  tmp: %5.1fC", info -> airTmp);
    sprintf(airHumidity,    "Air  hum: %5.1f%%", info -> airHum);
    sprintf(airTempStat,    "Tmp Stat: %6s", airTmpHigh ? "HIGH" : (airTmpLow ? "LOW" : "OK"));
    sprintf(airHumidityStat,"Hum Stat: %6s", airHumHigh ? "HIGH" : (airHumLow ? "LOW" : "OK"));

    ssd1306_display_text(&dev, 1, "Air info:", 9, false);
    ssd1306_display_text(&dev, 2, airTemp, 16, false);
    ssd1306_display_text(&dev, 3, airHumidity, 16, false);
    ssd1306_display_text(&dev, 5, airTempStat, 16, false);
    ssd1306_display_text(&dev, 6, airHumidityStat, 16, false);
}

void displayLightInfo(Info *info) {
    bool lightLvlLow = info -> lightVal < LOW_LIGHT;

    char lightLevel[17];
    char lightLevelStat[17];

    sprintf(lightLevel,     "Lght lvl: %5.1f%%", info -> lightVal);
    sprintf(lightLevelStat, "Lght Stat: %5s", lightLvlLow ? "LOW" : "OK");

    ssd1306_display_text(&dev, 1, "Light info:", 11, false);
    ssd1306_display_text(&dev, 2, lightLevel, 16, false);
    ssd1306_display_text(&dev, 4, lightLevelStat, 16, false);
}

void displayExit(int select) {
    ssd1306_display_text(&dev, 1, "Exit experiment?", 16, false);
    ssd1306_display_text(&dev, 2, " * Yes", 6, select == 0);
    ssd1306_display_text(&dev, 3, " * No", 5, select == 1);
}

int exitSelect() {
    clearScreen();
    int select = 0;
    while (1) {
        displayExit(select);
        if (getEnt()) {
            clearScreen(dev);
            resetBtns();
            return !select;
        }
        if (getSel()) {
            resetBtns();
            select++;
            select %= 2;
            vTaskDelay(DELAY(100));
        }
    }
}

int displayScreen(Info *info) {
    if (getEnt() && currentDisplay != EXPERIMENT_MENU) {
        resetBtns();
        clearScreen();
        currentDisplay = EXPERIMENT_MENU;
    } else if (getSel()) {
        resetBtns();
        clearScreen();
        currentDisplay ++;
        currentDisplay %= 4;
    }
    switch (currentDisplay) {
    case OVERVIEW:
        displayInfo(info);
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
    case EXPERIMENT_MENU:
        return 1; // Return to app_main
    }
    updateTick();
    return 0;
}

void displaySmallPlant(){
    ssd1306_bitmaps(&dev, 44, 16, &smallplant, 40, 48, false);
}

void displayMediumPlant(){
    ssd1306_bitmaps(&dev, 44, 16, &mediumplant, 40, 48, false);
}

void displayLargePlant(){
    ssd1306_bitmaps(&dev, 44, 16, &largeplant, 40, 48, false);
}