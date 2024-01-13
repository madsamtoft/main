#include "air_sensor.h"

void initAir() {
    //Initialize the sensor (shared i2c) only once after boot.
    i2c_dev_t dev = {0};
    ESP_ERROR_CHECK(am2320_shared_i2c_init(&dev, I2C_NUM));
}

void updateAir(Info *air) {
    i2c_dev_t dev = {0};

    //Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(am2320_shared_i2c_init(&dev, I2C_NUM));

    float temperature, humidity;
    
    esp_err_t res = am2320_get_rht(&dev, &temperature, &humidity);
    if (res == ESP_OK) {
        air -> airTmp = temperature;
        air -> airHum = humidity;
    } else {
        printf("Error reading air temperature: %d (%s)\n", res, esp_err_to_name(res));
    }
}