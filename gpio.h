#ifndef GPIO_H
#define GPIO_H

#include "config.h"

//Define GPIO pins
#define GPIO_BTN_ENTER 18
#define GPIO_BTN_SELECT 19
#define GPIO_LED_RED 8
#define GPIO_LED_GREEN 7

static int btnEnt = 0;
static int btnSel = 0;

void interruptEnter(void *arg);
void interruptSelect(void *arg);
int getEnt();
int getSel();
void resetBtns();
void initButtons();
void initLEDs();

#endif