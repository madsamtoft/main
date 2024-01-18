#include "error_led.h"
#include "display.h"

void initRGB_LED() {
    // Prepare and then apply the LEDC PWM timer configuration (one time can drive multiple channels)
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 1 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel_red = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_RED,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO_RED,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_red));

    ledc_channel_config_t ledc_channel_green = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_GREEN,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO_GREEN,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_green));

    ledc_channel_config_t ledc_channel_blue = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_BLUE,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO_BLUE,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_blue));
}

void setRGB(int r, int g, int b) {
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_RED, 8192 - r*32)); // rgb * 32 to go from 0-255 to 0-8191
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_GREEN, 8192 - g*32));
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_BLUE, 8192 - b*32));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_RED));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_GREEN));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_BLUE));
}

void callRGB(int index) {
    switch (index) {
        case ERROR_AIR_TMP:
            setRGB(255, 0, 0);
            break;
        case ERROR_AIR_HUM:
            setRGB(255, 150, 0);
            break;
        case ERROR_SOIL_TMP:
            setRGB(0, 255, 0);
            break;
        case ERROR_SOIL_HUM:
            setRGB(0, 102, 255);
            break;
        case ERROR_LIGHT_VAL:
            setRGB(204, 0, 255);
            break;
    }
}

void setStatusBits(Info *info) {
    statusBits = 0;
    statusBits += (info -> airTmp < LOW_AIR_TMP || info -> airTmp > HIGH_AIR_TMP) << ERROR_AIR_TMP;
    statusBits += (info -> soilTmp < LOW_SOIL_TMP || info -> soilTmp > HIGH_SOIL_TMP) << ERROR_SOIL_TMP;
    statusBits += (info -> airHum < LOW_AIR_HUM || info -> airHum > HIGH_AIR_HUM) << ERROR_AIR_HUM;
    statusBits += (info -> soilHum < LOW_SOIL_HUM || info -> soilHum > HIGH_SOIL_HUM) << ERROR_SOIL_HUM;
    statusBits += (info -> lightVal < DIM_LIGHT) << ERROR_LIGHT_VAL;
}

void resetStatusBits() {
    statusBits = 0;
}

void blinkErrors() {
    TickType_t startTimeTicks = xTaskGetTickCount();
    while (1) {
        for (int i = 0; i < sizeof(int)*8; i ++) {
            if (statusBits & (1<<i)) {
                callRGB(i);
                vTaskDelayUntil(&startTimeTicks, DELAY(1000));
            }
        }
        while (statusBits == 0) {
            setRGB(0,0,0);
            vTaskDelayUntil(&startTimeTicks, DELAY(1000));
        }
    }
}
