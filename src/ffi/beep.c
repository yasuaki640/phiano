#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

const int AMPLITUDE = 28000;
const int FREQUENCY = 44100;

void beep(double frequency, int duration) {
    int samples = (int)(duration * 0.001 * FREQUENCY);
    int16_t *buf = (int16_t *)malloc(samples * sizeof(int16_t));
    if (!buf) {
        fprintf(stderr, "Malloc failed\n");
        exit(1);
    }

    for (int i = 0; i < samples; i++) {
        double time = i / (double)FREQUENCY;
        buf[i] = (int16_t)(AMPLITUDE * sin(2.0f * M_PI * frequency * time));
    }

    SDL_AudioSpec beep_spec;
    beep_spec.freq = FREQUENCY;
    beep_spec.format = AUDIO_S16SYS;
    beep_spec.channels = 1;
    beep_spec.samples = samples;
    beep_spec.callback = NULL;
    beep_spec.userdata = NULL;

    if (SDL_OpenAudio(&beep_spec, NULL) < 0) {
        fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_PauseAudio(0);
    SDL_QueueAudio(1, buf, samples * sizeof(int16_t));

    SDL_Delay(duration);
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    free(buf);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }

    beep(442.0, 1500);  // 440 Hz for 1 second
    SDL_Quit();
    return 0;
}
