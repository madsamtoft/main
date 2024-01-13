#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h> // malloc
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

//Driver libraries
#include "driver/gpio.h" // For GPIO (Buttons and LEDs)
#include "driver/adc.h" // For ADC (Light)
#include "driver/i2c.h" // For I2C (Soil and Temp/Humidity)

//Temperature/humidity sensor library
#include <am2320.h>

//Stemma soil sensor library
#include "Adafruit_Stemma_soil_sensor.h"

//Display libraies
#include "ssd1306.h"
//#include "font8x8_basic.h"

#define I2C_MASTER_FREQ_HZ 75000 //Reduce it to 50000 if the temperature/umidity sensor fails
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_SDA_GPIO 2
#define I2C_MASTER_SCL_GPIO 3
#define I2C_NUM 0

//Macros
#define DELAY(ms) (ms) / portTICK_PERIOD_MS

//Threshhold
#define LOW_SOIL_HUM 670
#define HIGH_SOIL_HUM 900

#define LOW_SOIL_TMP 10
#define HIGH_SOIL_TMP 30

#define LOW_AIR_HUM 20 // Might not be needed
#define HIGH_AIR_HUM 40

#define LOW_AIR_TMP 10
#define HIGH_AIR_TMP 30

#define LOW_LIGHT 1000

typedef struct {
    float airTmp;
    float airHum;
    short soilHum;
    float soilTmp;
    short lightVal;
} Info;

static SSD1306_t dev;

void i2cConfig();
void updateInfo(Info *info);
void periodicRead(int time);

#endif