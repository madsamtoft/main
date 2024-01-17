#include "soil_sensor.h"

void initSoil() {
    //Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_shared_i2c_init());
}

void updateSoil(Info *soil) {
    uint16_t moisture_value = 0;
    float temperature_value = 0;
    int ret_moist = adafruit_stemma_soil_sensor_read_moisture(I2C_NUM, &moisture_value);
    int ret_temp = adafruit_stemma_soil_sensor_read_temperature(I2C_NUM, &temperature_value);

    ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_shared_i2c_init());

    if (ret_moist == ESP_OK && ret_temp == ESP_OK) {
        soil -> soilHum = moisture_value;
        soil -> soilTmp = temperature_value;
    } else {
        printf("Error reading soil data\n");
        soil -> soilHum = -1;
        soil -> soilTmp = -1;
    }    
}

float soilPercentage(Info *soil) {
    return ((soil -> soilHum - 300) * 100.) / (1023-300);
}