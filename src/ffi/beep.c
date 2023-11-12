#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#define AMPLITUDE 28000
#define FREQUENCY 44100

typedef struct {
  double frequency;
  int duration;
} Note;

typedef struct {
  int16_t *buf;
  int total_samples;
  int sample_index;
} AudioData;

void audio_callback(void *userdata, Uint8 *stream, int len) {
  AudioData *audio_data = (AudioData *)userdata;
  int16_t *buf = audio_data->buf;
  int total_samples = audio_data->total_samples;
  int sample_index = audio_data->sample_index;

  int16_t *stream16 = (int16_t *)stream;
  int stream_len = len / sizeof(int16_t);

  for (int i = 0; i < stream_len; i++) {
    if (sample_index >= total_samples) {
      break;
    }
    stream16[i] = buf[sample_index++];
  }

  audio_data->sample_index = sample_index;
}

void generate_samples(int16_t *buf, int num_samples, double frequency,
                      int duration) {
  for (int i = 0; i < num_samples; i++) {
    double time = i / (double)FREQUENCY;
    buf[i] = (int16_t)(AMPLITUDE * sin(2.0f * M_PI * frequency * time));
  }
}

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
    generate_samples(buf + sample_index, samples, note.frequency,
                     note.duration);
    sample_index += samples;
  }

  SDL_AudioSpec beep_spec;
  beep_spec.freq = FREQUENCY;
  beep_spec.format = AUDIO_S16SYS;
  beep_spec.channels = 1;
  beep_spec.samples = 4096;
  beep_spec.callback = audio_callback;

  AudioData audio_data;
  audio_data.buf = buf;
  audio_data.total_samples = total_samples;
  audio_data.sample_index = 0;

  beep_spec.userdata = &audio_data;

  if (SDL_OpenAudio(&beep_spec, NULL) < 0) {
    fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
    exit(1);
  }

  int total_duration = 0;
  for (int i = 0; i < num_notes; i++) {
    total_duration += notes[i].duration;
  }

  SDL_PauseAudio(0);
  SDL_Delay(total_duration);
  SDL_PauseAudio(1);
  SDL_CloseAudio();
  free(buf);
}
