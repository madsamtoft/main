#ifndef GPIO_H
#define GPIO_H

#include "config.h"

//Define GPIO pins
#define GPIO_BTN_ENTER 18
#define GPIO_BTN_SELECT 19
#define GPIO_LED_RED 7
#define GPIO_LED_GREEN 8 

void initButtons();
void initLEDs();

#endif