#include "light_sensor.h"

void initLight() {
    //Configuring the ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); //ADC1_CHANNEL_0 is on GPIO0 (GPIOzero)
}

void updateLight(Info *light) {
    light -> lightVal = (adc1_get_raw(ADC1_CHANNEL_0) * 100.) /4095;
}