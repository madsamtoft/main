#ifndef BUZZER_H
#define BUZZER_H

#include "main.h"

// necessary libraries !
#include <math.h>
#include "driver/ledc.h"

#define BUZZ_TIMER              LEDC_TIMER_1
#define BUZZ_MODE               LEDC_LOW_SPEED_MODE
#define BUZZ_OUTPUT_IO          (9) // Define the output GPIO for red
#define BUZZ_CHANNEL            LEDC_CHANNEL_4
#define BUZZ_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define BUZZ_DUTY               (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define BUZZ_FREQUENCY          (1000) // Frequency in Hertz. Set frequency at 1 kHz

#define C 16.352
#define D 18.354
#define Eb 19.445
#define E 20.602
#define F 21.827
#define Fb 23.124
#define G 24.5
#define Gb 25.957
#define A 27.5
#define B 29.135
#define H 30.868

#define TEMPO 200
#define OCTAVESHIFT 2

void initBuzzer();
void set_buzzer_volume(float volume);
void play_tone(int freq_hz, int time_ms);
float get_freq(float note, double octave);
void play_note(double note, double octave, double beats);
void melody_load();
void melody_load_done();
void melody_amogus();
void melody_error();
void sfx_1();
void sfx_2();
void sfx_3();
void titanic();

#endif