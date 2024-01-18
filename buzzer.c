#include "buzzer.h"

void initBuzzer() {
    // Function content from "ESP32-C3 Components Demo" on DTU-Learn
    // Prepare and then apply the LEDC PWM timer configuration (we use it for the buzzer)
    ledc_timer_config_t ledc_timer_buzz = {
        .speed_mode       = BUZZ_MODE,
        .duty_resolution  = BUZZ_DUTY_RES,
        .timer_num        = BUZZ_TIMER,
        .freq_hz          = BUZZ_FREQUENCY,  // Set output frequency at 1 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_buzz));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel_buzz = {
        .speed_mode     = BUZZ_MODE,
        .channel        = BUZZ_CHANNEL,
        .timer_sel      = BUZZ_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = BUZZ_OUTPUT_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_buzz));
}

void set_buzzer_volume(float volume) {
    //volume between 0 and 1
    volume = volume < 0 ? 0 : volume;
    volume = volume > 1 ? 1 : volume;
    int duty = volume * pow(2,13);
    ESP_ERROR_CHECK(ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, duty));
    ESP_ERROR_CHECK(ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL));
}

void play_tone(int freq_hz, int time_ms) {
    if (freq_hz != 0) {
        set_buzzer_volume(0.5);
        ESP_ERROR_CHECK(ledc_set_freq(BUZZ_MODE, BUZZ_TIMER, freq_hz));
        vTaskDelay((time_ms - 10) / portTICK_PERIOD_MS);
        set_buzzer_volume(0);
        vTaskDelay((10) / portTICK_PERIOD_MS);
    } else {
        set_buzzer_volume(0);
        vTaskDelay((time_ms) / portTICK_PERIOD_MS);
    }
}

float get_freq(float note, double octave) {
    return note * pow(2,octave + OCTAVESHIFT);
}

void play_note(double note, double octave, double beats) {
    // milliseconds = Beats / Beats per Minute * seconds per minute * milliseconds per second
    float time_ms = beats/TEMPO*60*1000;
    play_tone(get_freq(note,octave),time_ms);
}

void melody_load() {
    // float notes[] =   {C,D,E,0,C};
    // int octaves[] =   {3,3,3,0,4};
    // float lengths[] = {2,2,2,4,2};

    float notes[] =   {C,D,E};
    int octaves[] =   {3,3,3};
    float lengths[] = {2,2,2};

    int length = sizeof(octaves)/sizeof(int);
    for (int i = 0; i < length; i ++) {
        play_note(notes[i],octaves[i],lengths[i]/4);
    }
    vTaskDelete(NULL);
}

void melody_load_done() {
    play_note(C, 4, 0.5);
    vTaskDelete(NULL);
}

void melody_amogus() {
    float notes[] =   {C,Eb,F,Fb,F,Eb,C,0,Gb,Eb,C,0};
    int octaves[] =   {3,3, 3,3, 3,3, 3,0,2, 3, 3,0};
    float lengths[] = {4,4, 4,4, 4,4, 4,8,2, 2, 4,4};

    int length = sizeof(octaves)/sizeof(int);
    for (int i = 0; i < length; i ++) {
        play_note(notes[i],octaves[i],lengths[i]/4);
    }
    vTaskDelete(NULL);
}

void melody_moonlight() {
    float notes[] =   {A,D,F,
                       A,D,F,
                       A,D,F,
                       A,D,F,
                       A,D,F,
                       A,D,F,
                       A,D,F,
                       A,D,F,
                       B,D,F,
                       B,D,F,
                       B,Eb,G,
                       B,Eb,G,
                       A,D,F,
                       A,D,F,
                       A,Db,E,
                       A,Db,E,D};
    
    int octaves[] =   {3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4,
                       3,4,4};
    
    float lengths[] = {6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6,
                       6,6,6};

    int length = sizeof(octaves)/sizeof(int);
    for (int i = 0; i < length; i ++) {
        play_note(notes[i],octaves[i],lengths[i]/4);
    }
    vTaskDelete(NULL);
}

void melody_error() {
    float notes[] =   {C,Fb};
    int octaves[] =   {4,4};
    float lengths[] = {1,1};

    int length = sizeof(octaves)/sizeof(int);
    int repeats = 4; 
    for (int n = 0; n < repeats; n ++) {
        for (int i = 0; i < length; i ++) {
            play_note(notes[i],octaves[i],lengths[i]/4);
        }
    }
    vTaskDelete(NULL);
}

void sfx_1() {
    play_note(C,4,0.25);
    vTaskDelete(NULL);
}

void sfx_2() {
    play_note(G,3,0.5);
    vTaskDelete(NULL);
}

void sfx_3() {
    play_note(G,3,0.5);
    play_note(C,4,0.5);
    vTaskDelete(NULL);
}

void titanic(){
    float notes[] =   {F,G,G,A,  G,F,G,C,  B,A,F,D,  B,C,  F,G,G,A,  G,F,G,C,  A,C,D,C,  G,A,G,F}; //extension F,F,F,F, E,F,F,E,F,  0,G,A,G,  F,F,F,F, E,F,F,C
    int octaves[] =   {4,4,4,4,  4,4,4,5,  4,4,4,4,  3,4,  4,4,4,4,  4,4,4,5,  4,5,5,5,  4,4,4,4};// 4,4,4,4, 4,4,4,4,4,  0,4,4,4,  4,4,4,4, 4,4,4,4
    float lengths[] = {2,2,2,10, 2,2,2,10, 2,2,4,8,  4,10, 2,2,2,10, 2,2,2,10, 2,2,8,8,  1,1,14,10}; //6,2,4,4, 4,10,4,4,4, 4,4,10,10,6,2,4,4, 4,10,10,16

    int length = sizeof(octaves)/sizeof(int);
    for (int i = 0; i < length; i ++) {
        play_note(notes[i],octaves[i]-2,lengths[i]/2);
    }
    vTaskDelete(NULL);
}
