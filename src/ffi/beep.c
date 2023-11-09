#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

const int AMPLITUDE = 28000;
const int FREQUENCY = 44100;

typedef struct {
    double frequency;
    int duration;
} Note;

void beep(Note note) {
    printf("frequency: %f\n", note.frequency);
    printf("duration: %d\n", note.duration);
    int samples = (int)(note.duration * 0.001 * FREQUENCY);
    int16_t *buf = (int16_t *)malloc(samples * sizeof(int16_t));
    if (!buf) {
        fprintf(stderr, "Malloc failed\n");
        exit(1);
    }

    for (int i = 0; i < samples; i++) {
        double time = i / (double)FREQUENCY;
        buf[i] = (int16_t)(AMPLITUDE * sin(2.0f * M_PI * note.frequency * time));
    }

    SDL_AudioSpec beep_spec;
    beep_spec.freq = FREQUENCY;
    beep_spec.format = AUDIO_S16SYS;
    beep_spec.channels = 1;
    beep_spec.samples = samples; // TODO noteを合計したものに変える
    beep_spec.callback = NULL;
    beep_spec.userdata = NULL;

    if (SDL_OpenAudio(&beep_spec, NULL) < 0) {
        fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_PauseAudio(0);
    SDL_QueueAudio(1, buf, samples * sizeof(int16_t));

    SDL_Delay(note.duration);
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    free(buf);
}
