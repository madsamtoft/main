#include "gpio.h"

void buttonConfig() { // Configure buttons
    gpio_reset_pin(GPIO_BTN_ENTER); // Reset pin
    gpio_reset_pin(GPIO_BTN_SELECT);
	gpio_set_direction(GPIO_BTN_ENTER, GPIO_MODE_INPUT); // Set button as input
    gpio_set_direction(GPIO_BTN_SELECT, GPIO_MODE_INPUT);
    gpio_pullup_en(GPIO_BTN_ENTER); // Enable pull-up resistor
    gpio_pullup_en(GPIO_BTN_SELECT);
}