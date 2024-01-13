#include "gpio.h"

void initButtons() { // Configure buttons
    gpio_reset_pin(GPIO_BTN_ENTER); // Reset pin
    gpio_reset_pin(GPIO_BTN_SELECT);
	gpio_set_direction(GPIO_BTN_ENTER, GPIO_MODE_INPUT); // Set button as input
    gpio_set_direction(GPIO_BTN_SELECT, GPIO_MODE_INPUT);
    gpio_pullup_en(GPIO_BTN_ENTER); // Enable pull-up resistor
    gpio_pullup_en(GPIO_BTN_SELECT);
}

void initLEDs() { // Configure LEDs
    gpio_reset_pin(GPIO_LED_RED);
    gpio_reset_pin(GPIO_LED_GREEN);
    gpio_set_direction(GPIO_LED_RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_LED_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_LED_RED, 0);
    gpio_set_level(GPIO_LED_GREEN, 1);
}