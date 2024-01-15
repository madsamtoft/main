#include "main.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO_RED      (4) // Define the output GPIO for red
#define LEDC_OUTPUT_IO_GREEN    (5) // Define the output GPIO for green
#define LEDC_OUTPUT_IO_BLUE     (6) // Define the output GPIO for blue
#define LEDC_CHANNEL_RED        LEDC_CHANNEL_0
#define LEDC_CHANNEL_GREEN      LEDC_CHANNEL_1
#define LEDC_CHANNEL_BLUE       LEDC_CHANNEL_2
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define LEDC_FREQUENCY          (1000) // Frequency in Hertz. Set frequency at 1 kHz

static int statusBits = 0;

void initRGB_LED();
void setRGB(int r, int g, int b);
void callRGB(int index);
void setStatusBits(Info *info);
void blinkErrors();

enum ErrorCodes {
    ERROR_AIR_TMP,
    ERROR_SOIL_TMP,
    ERROR_AIR_HUM,
    ERROR_SOIL_HUM,
    ERROR_LIGHT_VAL
};