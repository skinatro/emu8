#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "util.h"

int main(int argc, char *argv[]) {
  chip8_t myChip8;
  SDLResources resources;

  // Checking if user passed any rom, if not then exit
  if (argc < 2) {
    fprintf(stderr, "Usage: emu8 <loadable rom>\n");
    return 1;
  }

  printf("Rom File -> %s\n", argv[1]);

  // Seed the rng
  srand(time(0));
  puts("Seeding rand with current time");

  InitSDL(&resources);
  initialize(&myChip8);
  load_rom(&myChip8, argv[1]);

  SDL_Rect rect = {.x = 0, .y = 0, .w = WINDOW_SCALE, .h = WINDOW_SCALE};

  int instructions_per_frame = IPF;

  SDL_AudioSpec wavSpec;
  uint32_t wavLength;
  uint8_t *wavBuffer;
  SDL_LoadWAV("./440.wav", &wavSpec, &wavBuffer, &wavLength);

  // Open audio device
  SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

  while (myChip8.running_flag) {
    uint64_t prexec_time = SDL_GetPerformanceCounter();

    if (myChip8.delay_timer > 0) {
      myChip8.delay_timer--;
    }
    printf("DT state - %d\n", myChip8.delay_timer);

    if (myChip8.sound_timer > 0) {  // beep if not zero
      myChip8.sound_timer--;        // decrement sound timer
      SDL_QueueAudio(dev, wavBuffer, wavLength);
      SDL_PauseAudioDevice(dev, 0);
    } else {
      SDL_PauseAudioDevice(dev, 1);
    }
    printf("ST state - %d \n", myChip8.sound_timer);

    handle_input(&myChip8);

    for (int i = 0; i < instructions_per_frame; i++) {
      cpu_cycle(&myChip8);
    }

    uint64_t postexec_time = SDL_GetPerformanceCounter();
    double elapsed_time = (double)((postexec_time - prexec_time) * 1000) /
                          SDL_GetPerformanceFrequency();
    SDL_Delay(16.67f > elapsed_time ? 16.67f - elapsed_time : 0);

    if (myChip8.draw_flag) {
      draw(&resources, &myChip8, &rect);
    }
  }
  SDL_CloseAudioDevice(dev);
  cleanup(&resources);
  return 0;
}
