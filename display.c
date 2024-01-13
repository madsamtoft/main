#include "display.h"
#include "gpio.h"

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
    ssd1306_display_text(&dev, 2, "-5 minutes", 10, (select == 0));
    ssd1306_display_text(&dev, 3, "-30 minutes", 11, (select == 1));
    ssd1306_display_text(&dev, 4, "-1 hour", 7, (select == 2));
    ssd1306_display_text(&dev, 5, "-2 hours", 8, (select == 3));
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
    ssd1306_display_text(&dev, 1, "Overview", 8, false);
    ssd1306_display_text(&dev, 2, airTemp, 16, false);
    ssd1306_display_text(&dev, 3, soilTemp, 16, false);
    ssd1306_display_text(&dev, 4, airHumidity, 16, false);
    ssd1306_display_text(&dev, 5, soilHumidity, 16, false);
    ssd1306_display_text(&dev, 6, lightLevel, 16, false);
}

void displayAverage(Info *info) {

}

void averageSelect() {
    int select = 0;
    while (1) {
        displayMenuAverage(select);
        if (gpio_get_level(GPIO_BTN_ENTER) == 0) {
            clearScreen(dev);
            if (select == 0) {
                periodicRead(5);
                return;
            } else if (select == 1) {
                periodicRead(1800);
                return;
            } else if (select == 2) {
                return;
                periodicRead(3600);
            } else if (select == 3) {
                periodicRead(7200);
                return;
            }
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

            switch (select) {
            case 0:
                currentDisplay = OVERVIEW;
                break;
            case 1:
                averageSelect();
                break;
            case 2:
                currentDisplay = SOIL_SENSOR;
                break;
            case 3:
                currentDisplay = AIR_SENSOR;
                break;
            case 4:
                currentDisplay = LIGHT_SENSOR;
                break;
            }
            return;
        }
        if (gpio_get_level(GPIO_BTN_SELECT) == 0) {
            select++;
            select %= 5;
            vTaskDelay(DELAY(100));
        }
    }
}

void displayScreen() {
    switch (currentDisplay) {
    case OVERVIEW:
        displayInfo();
        break;
    case AVERAGE_MENU:
        menuSelect();
        break;
    case SOIL_SENSOR:
        // displaySoilInfo();
        break;
    case AIR_SENSOR:
        // displayAirInfo();
        break;
    case LIGHT_SENSOR:
        // displayLightInfo();
        break;
    default:
        displayInfo();
        break;
    }
}