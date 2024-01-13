#include "display.h"
#include "gpio.h"
#include "buzzer.h"

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
    ssd1306_display_text(&dev, 3, "Get Average", 11, (select == 1));
    ssd1306_display_text(&dev, 4, "Soil Sensor", 11, (select == 2));
    ssd1306_display_text(&dev, 5, "Air Sensor", 10, (select == 3));
    ssd1306_display_text(&dev, 6, "Light Sensor", 12, (select == 4));
}

void displayMenuAverage(int select) {
    ssd1306_display_text(&dev, 1, "Get avg. over:", 14, false);
    ssd1306_display_text(&dev, 2, "-5 seconds", 10, (select == 0));
    ssd1306_display_text(&dev, 3, "-1 minute", 9, (select == 1));
    ssd1306_display_text(&dev, 4, "-5 minutes", 10, (select == 2));
    ssd1306_display_text(&dev, 5, "-1 hour", 7, (select == 3));
}

void displayInfo(Info *info) { // Method to display current info values
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
    ssd1306_display_text(&dev, 2, airTemp, 16, false);
    ssd1306_display_text(&dev, 3, soilTemp, 16, false);
    ssd1306_display_text(&dev, 4, airHumidity, 16, false);
    ssd1306_display_text(&dev, 5, soilHumidity, 16, false);
    ssd1306_display_text(&dev, 6, lightLevel, 16, false);
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


void averageSelect() {
    int select = 0;
    while (1) {
        displayMenuAverage(select);
        if (gpio_get_level(GPIO_BTN_ENTER) == 0) {
            // btnPressed = 0;
            clearScreen(dev);
            switch (select) {
            case AVERAGE_5SEC:
                periodicRead(5);
                break;
            case AVERAGE_1MIN:
                periodicRead(60);
                break;
            case AVERAGE_5MIN:
                periodicRead(300);
                break;
            case AVERAGE_1HOUR:
                periodicRead(6000);
                break;
            }
            currentDisplay = OVERVIEW;
            return;
        }
        if (gpio_get_level(GPIO_BTN_SELECT) == 0) {
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
        if (gpio_get_level(GPIO_BTN_ENTER) == 0) {
            clearScreen();
            xTaskCreate(sfx_2, "sfx_2", 1000, NULL, 1, NULL);
            currentDisplay = select;
            return;
        }
        if (gpio_get_level(GPIO_BTN_SELECT) == 0) {
            xTaskCreate(sfx_1, "sfx_1", 1000, NULL, 1, NULL);
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
    case AVERAGE_MENU:
        averageSelect();
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