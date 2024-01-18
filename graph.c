#include "graph.h"

void renderBitmapAirTmp(Info data[], int size, uint8_t *bitmap) {
    for (int i = 0; i < BITMAP_LENGTH; i++) {
        bitmap[i] = 0;
    }

    if (size > WIDTH) {
        for (int i = 0; i < size; i++) {
            int x = (float) i / size * WIDTH;
            float value = data[i].airTmp;
            int y = HEIGHT - (int) ((value-MIN_TEMP) * HEIGHT / (MAX_TEMP-MIN_TEMP));
            printf("Air: Adjusted temp: %.2f. Y-value: %d\n",value - MIN_TEMP, y);
            int bit = x + y * WIDTH;
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    } else {
        float scale = (float) size / WIDTH;
        for (int x = 0; x < WIDTH; x++) {     
            int i = (int) (scale * x);
            float value = data[i].airTmp;
            int y = HEIGHT - (int) ((value-MIN_TEMP) * HEIGHT / (MAX_TEMP-MIN_TEMP));
            int bit = x + y * WIDTH;
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    }
}

void renderBitmapSoilTmp(Info data[], int size, uint8_t *bitmap) {
    for (int i = 0; i < BITMAP_LENGTH; i++) {
        bitmap[i] = 0;
    }

    if (size > WIDTH) {
        for (int i = 0; i < size; i++) {
            int x = (float) i / size * WIDTH;
            float value = data[i].soilTmp;
            int y = HEIGHT - (int) ((value-MIN_TEMP) * HEIGHT / (MAX_TEMP-MIN_TEMP));
            printf("Soil: Adjusted temp: %.2f. Y-value: %d\n",value - MIN_TEMP, y);
            int bit = x + y * WIDTH;
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    } else {
        float scale = (float) size / WIDTH;
        for (int x = 0; x < WIDTH; x++) {     
            int i = (int) (scale * x);
            float value = data[i].soilTmp;
            int y = HEIGHT - (int) ((value-MIN_TEMP) * HEIGHT / (MAX_TEMP-MIN_TEMP));
            int bit = x + y * WIDTH;
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    }
}

void renderBitmapAirHum(Info data[], int size, uint8_t *bitmap) {
    for (int i = 0; i < BITMAP_LENGTH; i++) {
        bitmap[i] = 0;
    }

    if (size > WIDTH) {
        for (int i = 0; i < size; i++) {
            int x = (float) i / size * WIDTH;
            float value = data[i].airHum;
            int y = HEIGHT - (int) (value / 100 * HEIGHT);
            int bit = x + y * WIDTH;
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    } else {
        float scale = (float) size / WIDTH;
        for (int x = 0; x < WIDTH; x++) {     
            int i = (int) (scale * x);
            float value = data[i].airHum;
            int y = HEIGHT - (int) (value / 100 * HEIGHT);
            int bit = x + y * WIDTH;
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    }
}

void renderBitmapSoilHum(Info data[], int size, uint8_t *bitmap) {
    for (int i = 0; i < BITMAP_LENGTH; i++) {
        bitmap[i] = 0;
    }

    if (size > WIDTH) {
        for (int i = 0; i < size; i++) {
            int x = (float) i / size * WIDTH;
            float value = data[i].soilHum;
            int y = HEIGHT - (int) (value / 100 * HEIGHT);
            int bit = x + y * WIDTH;
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    } else {
        float scale = (float) size / WIDTH;
        for (int x = 0; x < WIDTH; x++) {     
            int i = (int) (scale * x);
            float value = data[i].soilHum;
            int y = HEIGHT - (int) (value / 100 * HEIGHT);
            int bit = x + y * WIDTH;
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    }
}

void renderBitmapLight(Info data[], int size, uint8_t *bitmap) {
    for (int i = 0; i < BITMAP_LENGTH; i++) {
        bitmap[i] = 0;
    }

    if (size > WIDTH) {
        for (int i = 0; i < size; i++) {
            int x = (float) i / size * WIDTH;
            float value = data[i].lightVal;
            int y = HEIGHT - (int) (value / 100 * HEIGHT);
            int bit = x + y * WIDTH;
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    } else {
        float scale = (float) size / WIDTH;
        for (int x = 0; x < WIDTH; x++) {     
            int i = (int) (scale * x);
            float value = data[i].lightVal;
            int y = HEIGHT - (int) (value / 100 * HEIGHT);
            int bit = x + y * WIDTH;
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    }
}

void percentAxis(SSD1306_t *dev, int time, char temp[]) {
    char line0[23];
    char line1[23];

    char displayChar = 's';
    if (time >= 60) {
        time = time / 60;
        displayChar = 'm';
    }

    sprintf(line0, "Time: %3d%c", time, displayChar);
    sprintf(line1, "%s graph", temp);

    ssd1306_display_text(dev, 0, line0, 10, false);
    ssd1306_display_text(dev, 1, line1, 16, false);
    ssd1306_display_text(dev, 2, "80|", 3, false);
    ssd1306_display_text(dev, 3, "60|", 3, false);
    ssd1306_display_text(dev, 4, "40|", 3, false);
    ssd1306_display_text(dev, 5, "20|", 3, false);
    ssd1306_display_text(dev, 6, " 0|", 3, false);
    ssd1306_display_text(dev, 7, "  -----Time-----", 16, false);
}

void temperatureAxis(SSD1306_t *dev, int time, char temp[]) {
    char line0[23];
    char line1[23];

    char displayChar = 's';
    if (time >= 60) {
        time = time / 60;
        displayChar = 'm';
    }

    sprintf(line0, "Time: %3d%c", time, displayChar);
    sprintf(line1, "%s graph", temp);

    ssd1306_display_text(dev, 0, line0, 10, false);
    ssd1306_display_text(dev, 1, line1, 16, false);
    ssd1306_display_text(dev, 2, "50|", 3, false);
    ssd1306_display_text(dev, 3, "35|", 3, false);
    ssd1306_display_text(dev, 4, "20|", 3, false);
    ssd1306_display_text(dev, 5, " 5|", 3, false);
    ssd1306_display_text(dev, 6, "-9|", 3, false);
    ssd1306_display_text(dev, 7, "  -----Time-----", 16, false);
}

void graphBitmap(SSD1306_t *dev, uint8_t *bitmap, int time, int display) {
    char line1[23] = "ERROR";
    switch (display) {
    case 0:
        sprintf(line1, "AirTemp");
        temperatureAxis(dev, time, line1);
        break;
    case 1:
        sprintf(line1, "SoilTemp");
        temperatureAxis(dev, time, line1);
        break;
    case 2:
        sprintf(line1, "AirHum");
        percentAxis(dev, time, line1);
        break;
    case 3:
        sprintf(line1, "SoilHum");
        percentAxis(dev, time, line1);
        break;
    case 4:
        sprintf(line1, "LightLvl");
        percentAxis(dev, time, line1);
        break;
    }
    ssd1306_bitmaps(dev, 24, 16, bitmap, WIDTH, HEIGHT, false);
}