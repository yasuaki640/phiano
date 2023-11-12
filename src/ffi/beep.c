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

void beep(Note notes[], int num_notes) {
    int total_samples = 0;
    for (int i = 0; i < num_notes; i++) {
        total_samples += (int)(notes[i].duration * 0.001 * FREQUENCY);
    }

    int16_t *buf = (int16_t *)malloc(total_samples * sizeof(int16_t));
    if (!buf) {
        fprintf(stderr, "Malloc failed\n");
        exit(1);
    }

    int sample_index = 0;
    for (int i = 0; i < num_notes; i++) {
        Note note = notes[i];
        int samples = (int)(note.duration * 0.001 * FREQUENCY);
        for (int j = 0; j < samples; j++) {
            double time = j / (double)FREQUENCY;
            buf[sample_index++] = (int16_t)(AMPLITUDE * sin(2.0f * M_PI * note.frequency * time));
        }
    }

    SDL_AudioSpec beep_spec;
    beep_spec.freq = FREQUENCY;
    beep_spec.format = AUDIO_S16SYS;
    beep_spec.channels = 1;
    beep_spec.samples = total_samples;
    beep_spec.callback = NULL;
    beep_spec.userdata = NULL;

    if (SDL_OpenAudio(&beep_spec, NULL) < 0) {
        fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_PauseAudio(0);
    SDL_QueueAudio(1, buf, total_samples * sizeof(int16_t));

    int total_duration = 0;
    for (int i = 0; i < num_notes; i++) {
        total_duration += notes[i].duration;
    }
    SDL_Delay(total_duration);
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    free(buf);
}
