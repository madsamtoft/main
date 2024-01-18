#include "graph.h"

// uint8_t* testBitmap = (uint8_t *) malloc(sizeof(uint8_t) * BITMAP_LENGTH);

void renderBitmapLight(Info data[], int size, uint8_t *bitmap) {
    for (int i = 0; i < BITMAP_LENGTH; i++) {
        // printf("Loop number: %d\n", i);
        // vTaskDelay(DELAY(20));
        bitmap[i] = 0;
    }

    // if (size > WIDTH) {
        for (int i = 0; i < size; i++) {
            float value = data[i].lightVal;
            int x = (float) i / size * WIDTH;
            int y = HEIGHT - (int) (value / 4096 * HEIGHT);

            int bit = x + y * WIDTH;
            // printf("%p\n", *(bitmap + (bit/8) * sizeof(uint8_t)));
            bitmap[bit/8] = bitmap[bit/8] | (1<<(bit%8));
            printf("Loop number: %d. Bitmap value: %d", i, bit);
        }
    // } else {
    //     float scale = (float) size / WIDTH;
    //     for (int x = 0; x < WIDTH; x++) {     
    //         int i = (int) (scale * x);
    //         float value = data[i].lightVal;
    //         int y = HEIGHT - (int) (value / 4096 * HEIGHT);

    //         int bit = x + y * WIDTH;
    //         *(bitmap[bit/8]) = *(bitmap[bit/8]) | (1<<(bit%8));
    //     }
    // }
}



void graphBitmap(SSD1306_t *dev, uint8_t *bitmap) {
    // clearScreen();
    
    ssd1306_bitmaps(dev, 24, 16, bitmap, WIDTH, HEIGHT, false);
}