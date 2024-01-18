#include "graph.h"

void renderBitmapLight(Info data[], int size, uint8_t *bitmap) {
    for (int i = 0; i < BITMAP_LENGTH; i++) {
        bitmap[i] = 0;
    }

    if (size > WIDTH) {
        for (int i = 0; i < size; i++) {
            float value = data[i].lightVal;
            int x = (float) i / size * WIDTH;
            int y = HEIGHT - (int) (value / 100 * HEIGHT);

            int bit = x + y * WIDTH;
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    } else { // Bug that shows inconsistent lines when drawing few data points
        float scale = (float) size / WIDTH;
        for (int x = 0; x < WIDTH; x++) {     
            int i = (int) (scale * x);
            float value = data[i].lightVal;
            int y = HEIGHT - (int) (value / 100 * HEIGHT);

            int bit = x + y * WIDTH;
            printf("X-POS (Column): %d. Y-POS (Row): %d. Data-Index: %d. Bytenumber: %d\n", x, y, i, bit / 8);
            bitmap[bit/8] = bitmap[bit/8] | (1<<(7 - bit%8));
        }
    }
}

// void percentAxis

void graphBitmap(SSD1306_t *dev, uint8_t *bitmap) {
    // clearScreen();
    ssd1306_bitmaps(dev, 24, 16, bitmap, WIDTH, HEIGHT, false);
}