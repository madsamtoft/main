#include "display.h"
#include "gpio.h"
#include "buzzer.h"
#include "error_led.h"

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
    ssd1306_display_text(&dev, 2, " * 5 seconds", 12, (select == EXP_5SEC));
    ssd1306_display_text(&dev, 3, " * 1 minute", 11, (select == EXP_1MIN));
    ssd1306_display_text(&dev, 4, " * 5 minutes", 12, (select == EXP_5MIN));
    ssd1306_display_text(&dev, 5, " * 30 minutes", 12, (select == EXP_30MIN));
    ssd1306_display_text(&dev, 6, " * 1 hour", 9, (select == EXP_1HOUR));
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
    sprintf(soilHumidity,   "Soil hum: %6d", info -> soilHum);
    sprintf(lightLevel,     "Lght lvl: %6d", info -> lightVal);

    ssd1306_display_text(&dev, 1, "Overview:", 9, false);
    ssd1306_display_text(&dev, 2, airTemp, 16, airTmpError && blink);
    ssd1306_display_text(&dev, 3, soilTemp, 16, soilTmpError && blink);
    ssd1306_display_text(&dev, 4, airHumidity, 16, airHumError && blink);
    ssd1306_display_text(&dev, 5, soilHumidity, 16, soilHumError && blink);
    ssd1306_display_text(&dev, 6, lightLevel, 16, lightError && blink);
}

void displayExperiment(Info *info, int expProg, int expTime) {
    char experiment[23];
    char airTemp[17];
    char soilTemp[17];
    char airHumidity[17];
    char soilHumidity[17];
    char lightLevel[17];

    short timeLeft = expTime - expProg;
    short timeLeftMin = timeLeft / 60;
    short percent = (short) ((expProg * 100.) / expTime);

    sprintf(experiment,     "Prog: %3d%%  %3d%c", percent, (timeLeft < 60 ? timeLeft : timeLeftMin), (timeLeft < 60 ? 's' : 'm'));
    sprintf(airTemp,        "Air  tmp: %5.1fC", info -> airTmp);
    sprintf(soilTemp,       "Soil tmp: %5.1fC", info -> soilTmp);
    sprintf(airHumidity,    "Air  hum: %5.1f%%", info -> airHum);
    sprintf(soilHumidity,   "Soil hum: %6d", info -> soilHum);
    sprintf(lightLevel,     "Lght lvl: %6d", info -> lightVal);

    ssd1306_display_text(&dev, 1, experiment, 16, false);
    ssd1306_display_text(&dev, 2, airTemp, 16, false);
    ssd1306_display_text(&dev, 3, soilTemp, 16, false);
    ssd1306_display_text(&dev, 4, airHumidity, 16, false);
    ssd1306_display_text(&dev, 5, soilHumidity, 16, false);
    ssd1306_display_text(&dev, 6, lightLevel, 16, false);
}

void displayExperimentAverage(Info *avg) {  
    char airTemp[17];
    char soilTemp[17];
    char airHumidity[17];
    char soilHumidity[17];
    char lightLevel[17];

    sprintf(airTemp,        "Air  tmp: %5.1fC", avg -> airTmp);
    sprintf(soilTemp,       "Soil tmp: %5.1fC", avg -> soilTmp);
    sprintf(airHumidity,    "Air  hum: %5.1f%%", avg -> airHum);
    sprintf(soilHumidity,   "Soil hum: %6d", avg -> soilHum);
    sprintf(lightLevel,     "Lght lvl: %6d", avg -> lightVal);

    ssd1306_display_text(&dev, 1, "Exp. Average:", 13, false);
    ssd1306_display_text(&dev, 2, airTemp, 16, false);
    ssd1306_display_text(&dev, 3, soilTemp, 16, false);
    ssd1306_display_text(&dev, 4, airHumidity, 16, false);
    ssd1306_display_text(&dev, 5, soilHumidity, 16, false);
    ssd1306_display_text(&dev, 6, lightLevel, 16, false);
}

void displayExperimentMin(Info *min) {
    char airTemp[17];
    char soilTemp[17];
    char airHumidity[17];
    char soilHumidity[17];
    char lightLevel[17];

    sprintf(airTemp,        "Air  tmp: %5.1fC", min -> airTmp);
    sprintf(soilTemp,       "Soil tmp: %5.1fC", min -> soilTmp);
    sprintf(airHumidity,    "Air  hum: %5.1f%%", min -> airHum);
    sprintf(soilHumidity,   "Soil hum: %6d", min -> soilHum);
    sprintf(lightLevel,     "Lght lvl: %6d", min -> lightVal);

    ssd1306_display_text(&dev, 1, "Minimum Values:", 15, false);
    ssd1306_display_text(&dev, 2, airTemp, 16, false);
    ssd1306_display_text(&dev, 3, soilTemp, 16, false);
    ssd1306_display_text(&dev, 4, airHumidity, 16, false);
    ssd1306_display_text(&dev, 5, soilHumidity, 16, false);
    ssd1306_display_text(&dev, 6, lightLevel, 16, false);
}

void displayExperimentMax(Info *max) {
    char airTemp[17];
    char soilTemp[17];
    char airHumidity[17];
    char soilHumidity[17];
    char lightLevel[17];

    sprintf(airTemp,        "Air  tmp: %5.1fC", max -> airTmp);
    sprintf(soilTemp,       "Soil tmp: %5.1fC", max -> soilTmp);
    sprintf(airHumidity,    "Air  hum: %5.1f%%", max -> airHum);
    sprintf(soilHumidity,   "Soil hum: %6d", max -> soilHum);
    sprintf(lightLevel,     "Lght lvl: %6d", max -> lightVal);

    ssd1306_display_text(&dev, 1, "Maximum Values:", 15, false);
    ssd1306_display_text(&dev, 2, airTemp, 16, false);
    ssd1306_display_text(&dev, 3, soilTemp, 16, false);
    ssd1306_display_text(&dev, 4, airHumidity, 16, false);
    ssd1306_display_text(&dev, 5, soilHumidity, 16, false);
    ssd1306_display_text(&dev, 6, lightLevel, 16, false);
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
    sprintf(soilHumidity,     "Soil hum: %6d", info -> soilHum);
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

    sprintf(lightLevel,     "Lght lvl: %6d", info -> lightVal);
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
    clearScreen(dev);
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
            case EXP_30MIN:
                periodicRead(1800);
                break;
            case EXP_1HOUR:
                periodicRead(3600);
                break;
            }
            clearScreen(dev);
            currentDisplay = OVERVIEW;
            return;
        }
        if (getSel()) {
            resetBtns();
            select++;
            select %= (int) sizeof(enum experiment) + 1; 
            vTaskDelay(DELAY(100));
        }
    }
}

Info getAverage(Info data[], int size) {
    Info avg;
    avg.airTmp = 0;
    avg.soilTmp = 0;
    avg.airHum = 0;
    avg.soilHum = 0;
    avg.lightVal = 0;

    for (int i = 0; i < size; i++) {
        avg.airTmp += (data[i]).airTmp;
        avg.soilTmp += (data[i]).soilTmp;
        avg.airHum += (data[i]).airHum;
        avg.soilHum += (data[i]).soilHum;
        avg.lightVal += (data[i]).lightVal;
    }

    avg.airTmp = avg.airTmp / size;
    avg.soilTmp = avg.soilTmp / size;
    avg.airHum = avg.airHum / size;
    avg.soilHum = avg.soilHum / size;
    avg.lightVal = avg.lightVal / size;

    return avg;
}

Info getMin(Info data[], int size) {
    Info min;
    min.airTmp = data[0].airTmp;
    min.soilTmp = data[0].soilTmp;
    min.airHum = data[0].airHum;
    min.soilHum = data[0].soilHum;
    min.lightVal = data[0].lightVal;

    for(int i = 0; i < size; i++) {
        if(data[i].airTmp < min.airTmp) {
            min.airTmp = (data[i]).airTmp;
        }
        if(data[i].soilTmp < min.soilTmp) {
            min.soilTmp = (data[i]).soilTmp;
        }
        if(data[i].airHum < min.airHum) {
            min.airHum = (data[i]).airHum;
        }
        if(data[i].soilHum < min.soilHum) {
            min.soilHum = (data[i]).soilHum;
        }
        if(data[i].lightVal < min.lightVal) {
            min.lightVal = (data[i]).lightVal;
        }
    }

    return min;
}

Info getMax(Info data[], int size) {
    Info max;
    max.airTmp = data[0].airTmp;
    max.soilTmp = data[0].soilTmp;
    max.airHum = data[0].airHum;
    max.soilHum = data[0].soilHum;
    max.lightVal = data[0].lightVal;

    for (int i = 0; i < size; i++) {
        if(data[i].airTmp > max.airTmp) {
            max.airTmp = data[i].airTmp;
        }
        if(data[i].soilTmp > max.soilTmp) {
            max.soilTmp = data[i].soilTmp;
        }
        if(data[i].airHum > max.airHum) {
            max.airHum = data[i].airHum;
        }
        if(data[i].soilHum > max.soilHum) {
            max.soilHum = data[i].soilHum;
        }
        if(data[i].lightVal > max.lightVal) {
            max.lightVal = data[i].lightVal;
        }
    }

    return max;
}

void experimentResultsSelect(Info data[], int size) {
    Info avg = getAverage(data, size);
    Info min = getMin(data, size);
    Info max = getMax(data, size);

    int select = 0;
    clearScreen(dev);
    while (1) {
        switch (select) {
        case 0:
            displayExperimentAverage(&avg);
            break;
        case 1:
            displayExperimentMin(&min);
            break;
        case 2:
            displayExperimentMax(&max);
            break;
        }
        if (getEnt()) {
            return;
        }
        if (getSel()) {
            resetBtns();
            clearScreen(dev);
            select++;
            select %= 3;
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