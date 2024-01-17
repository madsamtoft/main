#ifndef GRAPH_H
#define GRAPH_H

//Own libraries
#include "main.h"
#include "gpio.h"
#include "experiment.h"
#include "display.h"

#define WIDTH 104
#define HEIGHT 40
#define BITMAP_LENGTH  (WIDTH * HEIGHT) / 8

void renderBitmapLight(uint8_t *bitmap, Info data[], int size);
void graphBitmap(SSD1306_t *dev, uint8_t *bitmap);


#endif