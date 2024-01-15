#include "gpio.h"
#include "buzzer.h"

void interruptEnter(void *arg) {
    xTaskCreate(sfx_2, "sfx_2", 1000, NULL, 1, NULL);
    btnEnt = 1;
}

void interruptSelect(void *arg) {
    xTaskCreate(sfx_1, "sfx_2", 1000, NULL, 1, NULL);
    btnSel = 1;
}

int getEnt() {
    return btnEnt;
}

int getSel() {
    return btnSel;
}

void resetBtns() {
    btnEnt = 0;
    btnSel = 0;
}

void initButtons() { // Configure buttons
    gpio_install_isr_service(0);
    gpio_reset_pin(GPIO_BTN_ENTER); // Reset pin
    gpio_reset_pin(GPIO_BTN_SELECT);
	gpio_set_direction(GPIO_BTN_ENTER, GPIO_MODE_INPUT); // Set button as input
    gpio_set_direction(GPIO_BTN_SELECT, GPIO_MODE_INPUT);
    gpio_pullup_en(GPIO_BTN_ENTER); // Enable pull-up resistor
    gpio_pullup_en(GPIO_BTN_SELECT);
    gpio_set_intr_type(GPIO_BTN_ENTER, GPIO_INTR_LOW_LEVEL); // Set interrupt type
    gpio_set_intr_type(GPIO_BTN_SELECT, GPIO_INTR_LOW_LEVEL);
    gpio_isr_handler_add(GPIO_BTN_ENTER, interruptEnter, NULL); // Add handler for button interrupt
    gpio_isr_handler_add(GPIO_BTN_SELECT, interruptSelect, NULL);
}

void initLEDs() { // Configure LEDs
    gpio_reset_pin(GPIO_LED_RED);
    gpio_reset_pin(GPIO_LED_GREEN);
    gpio_set_direction(GPIO_LED_RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_LED_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_LED_RED, 0);
    gpio_set_level(GPIO_LED_GREEN, 1);
}