#include "config.h"
#include "display.h"
#include "console.h"
#include "air_sensor.h"
#include "soil_sensor.h"
#include "light_sensor.h"

void buttonConfig() { // Configure buttons
    gpio_reset_pin(GPIO_BTN_ENTER); // Reset pin
    gpio_reset_pin(GPIO_BTN_SELECT);
	gpio_set_direction(GPIO_BTN_ENTER, GPIO_MODE_INPUT); // Set button as input
    gpio_set_direction(GPIO_BTN_SELECT, GPIO_MODE_INPUT);
    gpio_pullup_en(GPIO_BTN_ENTER); // Enable pull-up resistor
    gpio_pullup_en(GPIO_BTN_SELECT);
}

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
    for (int i = 0; i < time; i++) {
        updateInfo(&(data[i]));
        printInfo(&(data[i]));
        displayInfo(&(data[i]));
        vTaskDelay(DELAY(735)); // The time it takes to execute one iteration is 265ms so this is 1 read per second
    }
    printData(data, time);
    free(data); // Maybe needs to be moved if we want to use the array more
}