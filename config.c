#include "config.h"
#include "display.h"
#include "console.h"
#include "air_sensor.h"
#include "soil_sensor.h"
#include "light_sensor.h"

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

void periodicRead(int time) { // Read and get average over a period of time
    printf("Reading data for %d seconds:\n", time);
    Info *data = (Info *) malloc(sizeof(Info) * time);
    TickType_t startTimeTicks = xTaskGetTickCount();
    for (int i = 0; i < time; i++) {
        updateInfo(&(data[i]));
        printInfo(&(data[i]));
        displayInfo(&(data[i]));
        vTaskDelayUntil(&startTimeTicks, DELAY(1000));
    }
    printData(data, time);
    free(data); // Maybe needs to be moved if we want to use the array more
}