//Own libraries
#include "main.h"
#include "air_sensor.h"
#include "soil_sensor.h"
#include "light_sensor.h"
#include "display.h"
#include "console.h"
#include "buzzer.h"
#include "gpio.h"
#include "error_led.h"

void i2cConfig() {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_GPIO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_GPIO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = 0;
    i2c_param_config(I2C_NUM, &conf);
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));
}

void updateInfo(Info *info) {
    updateAir(info);
    updateSoil(info);
    updateLight(info);
}

void updateStat(float count, float value, Stat *stat) {
    if (count == 0) {
        stat->avg = value;
        stat->min = value;
        stat->max = value;
        return;
    }
    float a = count / (count + 1);
    float b = 1.0 / (count + 1);
    stat->avg = stat->avg * a + value * b;
    stat->min = fmin(stat->min, value);
    stat->max = fmax(stat->max, value);
}

void updateInfoStat(Info *info, InfoStat *infoStat) {
    updateStat(infoStat->count, info->airTmp, &(infoStat->airTmp));
    updateStat(infoStat->count, info->airHum, &(infoStat->airHum));
    updateStat(infoStat->count, info->soilHum, &(infoStat->soilHum));
    updateStat(infoStat->count, info->soilTmp, &(infoStat->soilTmp));
    updateStat(infoStat->count, info->lightVal, &(infoStat->lightVal));
    infoStat->count ++;
}

void periodicRead(int time) { // Read and get average over a period of time
    printf("Reading data for %d seconds:\n", time);

    // LED's
    gpio_set_level(GPIO_LED_GREEN, 0);
    int level = 1;
    resetStatusBits(); // Reset error LED
    
    // Prepare Array
    Info *data = (Info *) malloc(sizeof(Info) * time);

    // Task Handling
    TickType_t startTimeTicks = xTaskGetTickCount();

    int i;
    for (i = 0; i < time; i++) {
        if (getEnt()) {
            resetBtns();
            if (exitSelect()) {
                break;
            } else {
                startTimeTicks = xTaskGetTickCount();
            }
        }
        
        // LED's
        gpio_set_level(GPIO_LED_RED, level);
        level = !level;

        // INFO
        updateInfo(&(data[i]));
        // printInfo(&(data[i]));
        displayExperiment(&(data[i]), i, time);

        // Task Handling
        vTaskDelayUntil(&startTimeTicks, DELAY(1000));
    }
    // LED's
    gpio_set_level(GPIO_LED_RED, 0);
    gpio_set_level(GPIO_LED_GREEN, 1);

    // Sound Effect
    xTaskCreate(sfx_3, "sfx_3", 1000, NULL, 1, NULL);

    if(i == (time)) {
        displayExperiment(&(data[time - 1]), time, time);
        printData(data, time);
        experimentResultsSelect(data, time);
    }
    free(data); // Maybe needs to be moved if we want to use the array more
}

void mainTask() {
    Info current;
    InfoStat averages;
    averages.count = 0;

    // Error blink. will remain alive for ever. 
    xTaskCreate(blinkErrors, "blinkErrors", 1000, 1+2+4+8+16, 1, NULL);
    //Boot melody
    xTaskCreate(melody_load, "melody_load", 1000, NULL, 1, NULL);

    TickType_t startTimeTicks = xTaskGetTickCount();
    while (1) {
        if (getEnt()) {
            resetBtns();
            clearScreen();
            menuSelect();
        } else {
            startTimeTicks = xTaskGetTickCount();
        }
        updateInfo(&current);
        updateInfoStat(&current, &averages);
        setStatusBits(&current);
        printInfo(&current);
        displayScreen(&current);
        vTaskDelayUntil(&startTimeTicks, DELAY(1000));
    }
}

void app_main(void) {
    i2cConfig();
    initSoil();
    initLight();
    initButtons();
    initLEDs();
    initBuzzer();
    initDisplay();
    initRGB_LED();

    xTaskCreatePinnedToCore(&mainTask, "MainTask", 100000, NULL, 1, NULL, 0);    
}
