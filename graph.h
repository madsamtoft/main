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
#define MIN_TEMP (-10.0)
#define MAX_TEMP (50.0)

void renderBitmapAirTmp(Info data[], int size, uint8_t *bitmap);
void renderBitmapSoilTmp(Info data[], int size, uint8_t *bitmap);
void renderBitmapAirHum(Info data[], int size, uint8_t *bitmap);
void renderBitmapSoilHum(Info data[], int size, uint8_t *bitmap);
void renderBitmapLight(Info data[], int size, uint8_t *bitmap);
void percentAxis(SSD1306_t *dev, int time, char temp[]);
void temperatureAxis(SSD1306_t *dev, int time, char temp[]);
void graphBitmap(SSD1306_t *dev, uint8_t *bitmap, int time, int display);

#endif