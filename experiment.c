#include "experiment.h"

void initDisplayExp() {
    i2c_master_shared_i2c_init(&dev);
    ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);
}

void experimentSelect() {
    Info *exp;
    int time = 0;
    int select = 0;
    while (1) {
        displayMenuExperiment(select);
        if (getEnt()) {
            resetBtns();
            clearScreen();
            switch (select) {
            case EXP_5SEC:
                time = 5;
                break;
            case EXP_1MIN:
                time = 60;
                break;
            case EXP_5MIN:
                time = 300;
                break;
            case EXP_30MIN:
                time = 1800;
                break;
            case EXP_1HOUR:
                time = 3600;
                break;
            case EXP_FOREVER:
                foreverRead();
                break;
            }
            // Show results of experiment
            exp = (Info *) malloc(sizeof(Info) * time);
            if(time > 0 && periodicRead(exp, time)) {
                experimentResultsSelect(exp, time);
            }
            free(exp);
            // Return to overview
            clearScreen();
            updateCurrentDisplay(OVERVIEW);
            updateTick();
            return;
        }
        if (getSel()) {
            resetBtns();
            select++;
            select %= 6; 
            vTaskDelay(DELAY(100));
        }
    }
}

int periodicRead(Info *exp, int time) { // Read and get average over a period of time
    printf("Reading data for %d seconds:\n", time);
    printf("Time;airTMP;soilTMP;airHUM;soilHUM;light\n");
    // LED's
    int state = 1;
    resetStatusBits(); // Reset error LED
    // Task Handling
    TickType_t delayTicks = xTaskGetTickCount();

    int i;
    for (i = 0; i < time; i++) {
        if (getEnt()) {
            resetBtns();
            if (exitSelect()) {
                break;
            } else {
                delayTicks = xTaskGetTickCount();
            }
        }
        // Make LED flash
        gpio_set_level(GPIO_LED_RED, state);
        state = !state;
        // INFO
        updateInfo(&(exp[i]));
        printForever(exp[i], i);
        displayExperiment(&(exp[i]), i, time);
        // Task Handling
        vTaskDelayUntil(&delayTicks, DELAY(1000));
    }
    // LED's
    gpio_set_level(GPIO_LED_RED, 0);
    // Sound Effect
    xTaskCreate(sfx_3, "sfx_3", 1000, NULL, 1, NULL);
    // If experiment was stopped early we return 0, otherwise 1
    if(i == time) {
        displayExperiment(&(exp[time - 1]), time, time);
        return 1;
    } else {
        return 0;
    }
}

void foreverRead() {
    printf("Reading data until stopped:\n");
    printf("Time;airTMP;soilTMP;airHUM;soilHUM;light\n");
    int state = 1;
    int count = 0;
    Info exp;
    resetStatusBits(); // Reset error LED
    // Task Handling
    TickType_t delayTicks = xTaskGetTickCount();
    vTaskDelay(DELAY(1000));
    
    while (getEnt() == 0) {
        // Make LED flash
        gpio_set_level(GPIO_LED_RED, state);
        state = !state;
        // INFO
        updateInfo(&(exp));
        printForever(exp, count);
        displayForever(&(exp), count);
        // Task Handling
        vTaskDelayUntil(&delayTicks, DELAY(1000));
        count++;
    }
    // LED's
    gpio_set_level(GPIO_LED_RED, 0);
    // Sound Effect
    xTaskCreate(sfx_3, "sfx_3", 1000, NULL, 1, NULL);
}

void experimentResultsSelect(Info data[], int size) {
    Info avg = getAvg(data, size);
    Info min = getMin(data, size);
    Info max = getMax(data, size);
    int select = 0;
    clearScreen(dev);
    while (1) {
        switch (select) {
        case 0:
            displayExperimentAvg(&avg);
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
            select++;
            select %= 3;
            vTaskDelay(DELAY(100));
        }
    }
}

void displayMenuExperiment(int select) {
    ssd1306_display_text(&dev, 1, "Run exp. for:", 13, false);
    ssd1306_display_text(&dev, 2, " * 5 seconds", 12, (select == EXP_5SEC));
    ssd1306_display_text(&dev, 3, " * 1 minute", 11, (select == EXP_1MIN));
    ssd1306_display_text(&dev, 4, " * 5 minutes", 12, (select == EXP_5MIN));
    ssd1306_display_text(&dev, 5, " * 30 minutes", 12, (select == EXP_30MIN));
    ssd1306_display_text(&dev, 6, " * 1 hour", 9, (select == EXP_1HOUR));
    ssd1306_display_text(&dev, 7, " * Forever", 10, (select == EXP_FOREVER));
}

void displayForever(Info *info, int expProg) {
    char time[23];
    char airTemp[17];
    char soilTemp[17];
    char airHumidity[17];
    char soilHumidity[17];
    char lightLevel[17];

    int expProgMin = expProg / 60;
    int expProgHr = expProgMin / 60;

    int timeElapsed = expProg < 60 ? expProg : (expProgMin < 60 ? expProgMin : expProgHr);
    char timeSign = expProg < 60 ? 's' : (expProgMin < 60 ? 'm' : 'h');

    sprintf(time,           "Elapsed: %6d%c", timeElapsed, timeSign);
    sprintf(airTemp,        "Air  tmp: %5.1fC", info -> airTmp);
    sprintf(soilTemp,       "Soil tmp: %5.1fC", info -> soilTmp);
    sprintf(airHumidity,    "Air  hum: %5.1f%%", info -> airHum);
    sprintf(soilHumidity,   "Soil hum: %6d", info -> soilHum);
    sprintf(lightLevel,     "Lght lvl: %6d", info -> lightVal);

    ssd1306_display_text(&dev, 1, time, 16, false);
    ssd1306_display_text(&dev, 2, airTemp, 16, false);
    ssd1306_display_text(&dev, 3, soilTemp, 16, false);
    ssd1306_display_text(&dev, 4, airHumidity, 16, false);
    ssd1306_display_text(&dev, 5, soilHumidity, 16, false);
    ssd1306_display_text(&dev, 6, lightLevel, 16, false);
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

    short displayTime = timeLeft < 60 ? timeLeft : timeLeftMin;
    char displayChar = timeLeft < 60 ? 's' : 'm';

    sprintf(experiment,     "Prog: %3d%%  %3d%c", percent, displayTime, displayChar);
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

Info getAvg(Info data[], int size) {
    Info avg;
    float airTmp = 0;
    float soilTmp = 0;
    float airHum = 0;
    float soilHum = 0;
    float lightVal = 0;
    for (int i = 0; i < size; i++) {
        airTmp += (data[i]).airTmp;
        soilTmp += (data[i]).soilTmp;
        airHum += (data[i]).airHum;
        soilHum += (data[i]).soilHum;
        lightVal += (data[i]).lightVal;
    }
    avg.airTmp = airTmp / size;
    avg.soilTmp = soilTmp / size;
    avg.airHum = (int) airHum / size;
    avg.soilHum = soilHum / size;
    avg.lightVal = (int) lightVal / size;
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

void displayExperimentAvg(Info *avg) {  
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

    ssd1306_display_text(&dev, 1, "Average Values:", 15, false);
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

