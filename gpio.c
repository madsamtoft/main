#include "gpio.h"

void interruptHandler() {
    btnPressed = 1;
}

void initButtons() { // Configure buttons
    gpio_install_isr_service(0);
    gpio_reset_pin(GPIO_BTN_ENTER); // Reset pin
    gpio_reset_pin(GPIO_BTN_SELECT);
	gpio_set_direction(GPIO_BTN_ENTER, GPIO_MODE_INPUT); // Set button as input
    gpio_set_direction(GPIO_BTN_SELECT, GPIO_MODE_INPUT);
    gpio_pullup_en(GPIO_BTN_ENTER); // Enable pull-up resistor
    gpio_pullup_en(GPIO_BTN_SELECT);
    gpio_set_intr_type(GPIO_BTN_ENTER, GPIO_INTR_POSEDGE); // Set interrupt on positive edge
    gpio_isr_handler_add(GPIO_BTN_ENTER, interruptHandler, NULL); // Add handler for button interrupt
}

void initLEDs() { // Configure LEDs
    gpio_reset_pin(GPIO_LED_RED);
    gpio_reset_pin(GPIO_LED_GREEN);
    gpio_set_direction(GPIO_LED_RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_LED_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_LED_RED, 0);
    gpio_set_level(GPIO_LED_GREEN, 1);
}