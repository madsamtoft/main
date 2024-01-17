//Own libraries
#include "main.h"
#include "air_sensor.h"
#include "soil_sensor.h"
#include "light_sensor.h"
#include "display.h"
#include "experiment.h"
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

void updateTick() {
    startTimeTicks = xTaskGetTickCount();
}

void mainTask() {
    Info current;
    InfoStat averages;
    averages.count = 0;

    // Error blink. will remain alive for ever. 
    xTaskCreate(blinkErrors, "blinkErrors", 1000, 1+2+4+8+16, 1, NULL);
    //Boot melody
    xTaskCreate(melody_load, "melody_load", 1000, NULL, 1, NULL);

    updateTick();
    while (1) {
        updateInfo(&current);
        updateInfoStat(&current, &averages);
        setStatusBits(&current);
        if (displayScreen(&current)) {
            experimentSelect();
        }
        printInfo(&current);
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
    initDisplayExp();
    initRGB_LED();

    xTaskCreatePinnedToCore(&mainTask, "MainTask", 100000, NULL, 1, NULL, 0);    
}
