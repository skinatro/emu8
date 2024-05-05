#include "util.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void InitSDL(SDLResources *resources) {
  if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0) {
    SDL_Log("Initialization Error: %s", SDL_GetError());
    exit(69);
  }

  resources->window = SDL_CreateWindow(
      "EMU8 -", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE, 0);
  if (resources->window == NULL) {
    SDL_Log("Window Creation Error: %s", SDL_GetError());
    exit(70);
  }

  resources->renderer =
      SDL_CreateRenderer(resources->window, -1, SDL_RENDERER_ACCELERATED);
  if (resources->renderer == NULL) {
    SDL_Log("Renderer Creation Error: %s", SDL_GetError());
    exit(71);
  }
  SDL_SetRenderDrawColor(resources->renderer, RENDERER_R, RENDERER_G,
                         RENDERER_B, RENDERER_A);
}

// void audio_cb(void *userdata, unsigned short *raw_buf, int bytes) {
//   unsigned int len = (unsigned int)(bytes / 2);
//   unsigned long long start_sample = (unsigned long long)userdata;
//   double sample_timestep = 1.0f / AUDIO_SAMPLING_RATE;
//   unsigned long long current_time = start_sample / AUDIO_SAMPLING_RATE;

//   for (unsigned int w = 0; w < len; w++) {
//     raw_buf[w] = (unsigned short)(AUDIO_AMPLITUDE *
//                                   sin(2.0f * M_PI * 441.0f * current_time));
//     current_time += (unsigned long long)sample_timestep;
//   }

//   *(unsigned int *)userdata += len;
// }

void cleanup(SDLResources *resources) {
  SDL_RenderClear(resources->renderer);
  SDL_DestroyRenderer(resources->renderer);
  SDL_DestroyWindow(resources->window);
  SDL_Quit();

  exit(EXIT_SUCCESS);
}

void load_rom(chip8_t *myChip8, const char *filename) {
  printf("%s passed as argument\n", filename);

  // Open the rom in binary mode and check if it's valid
  FILE *rom = fopen(filename, "rb");
  if (rom == NULL) {
    perror("Failed to open file");
    exit(2);
  }
  printf("Found %s\n", filename);

  /*
      First Argument is the structure you want to store data in
      Here mem + ROM is pointer arithmetic
      It is saying that in array mem[], elements are stored starting 0x200
      Second is the size of each element
      Third is the maximum number of elements
      Fourth is the input stream
   */

  fseek(rom, 0, SEEK_END);
  int size = ftell(rom);
  if (size > RAM_MAX - RAM_START) {
    perror("Rom Size too big");
    exit(3);
  }
  fseek(rom, 0, SEEK_SET);
  fread(myChip8->memory + RAM_START, 1, RAM_MAX - RAM_START, rom);
  printf("Loaded %s successfully\n", filename);

  fclose(rom);
  printf("Closed %s stream\n", filename);
}

void draw(SDLResources *resources, chip8_t *myChip8, SDL_Rect *rect) {
  SDL_SetRenderDrawColor(resources->renderer, RENDERER_R, RENDERER_G,
                         RENDERER_B, RENDERER_A);
  SDL_RenderClear(
      resources->renderer);  // Clear the renderer with the current draw color

  for (int i = 0; i < DISPLAY_X * DISPLAY_Y; i++) {
    rect->x = (i % WINDOW_WIDTH) * WINDOW_SCALE;
    rect->y = (i / WINDOW_WIDTH) * WINDOW_SCALE;

    if (myChip8->pixels[i]) {
      SDL_SetRenderDrawColor(resources->renderer, RECT_R, RECT_G, RECT_B,
                             RECT_A);
    } else {
      SDL_SetRenderDrawColor(resources->renderer, RENDERER_R, RENDERER_G,
                             RENDERER_B, RENDERER_A);
    }
    SDL_RenderFillRect(resources->renderer, rect);
  }
  myChip8->draw_flag = 0;  // Reset the draw flag after drawing
  SDL_RenderPresent(
      resources->renderer);  // Update the screen with the rendered content
}

// Handle user input
// CHIP8 Keypad  QWERTY
// 123C          1234
// 456D          qwer
// 789E          asdf
// A0BF          zxcv

void handle_input(chip8_t *chip8) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        // Exit window; End program
        chip8->running_flag = 0;  // Will exit main emulator loop
        break;

      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            // Escape key; Exit window & End program
            chip8->running_flag = 0;
            break;

          // Map qwerty keys to CHIP8 keypad
          case SDLK_1:
            chip8->keypad[0x1] = true;
            break;
          case SDLK_2:
            chip8->keypad[0x2] = true;
            break;
          case SDLK_3:
            chip8->keypad[0x3] = true;
            break;
          case SDLK_4:
            chip8->keypad[0xC] = true;
            break;

          case SDLK_q:
            chip8->keypad[0x4] = true;
            break;
          case SDLK_w:
            chip8->keypad[0x5] = true;
            break;
          case SDLK_e:
            chip8->keypad[0x6] = true;
            break;
          case SDLK_r:
            chip8->keypad[0xD] = true;
            break;

          case SDLK_a:
            chip8->keypad[0x7] = true;
            break;
          case SDLK_s:
            chip8->keypad[0x8] = true;
            break;
          case SDLK_d:
            chip8->keypad[0x9] = true;
            break;
          case SDLK_f:
            chip8->keypad[0xE] = true;
            break;

          case SDLK_z:
            chip8->keypad[0xA] = true;
            break;
          case SDLK_x:
            chip8->keypad[0x0] = true;
            break;
          case SDLK_c:
            chip8->keypad[0xB] = true;
            break;
          case SDLK_v:
            chip8->keypad[0xF] = true;
            break;

          default:
            break;
        }
        break;

      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
          // Map qwerty keys to CHIP8 keypad
          case SDLK_1:
            chip8->keypad[0x1] = false;
            break;
          case SDLK_2:
            chip8->keypad[0x2] = false;
            break;
          case SDLK_3:
            chip8->keypad[0x3] = false;
            break;
          case SDLK_4:
            chip8->keypad[0xC] = false;
            break;

          case SDLK_q:
            chip8->keypad[0x4] = false;
            break;
          case SDLK_w:
            chip8->keypad[0x5] = false;
            break;
          case SDLK_e:
            chip8->keypad[0x6] = false;
            break;
          case SDLK_r:
            chip8->keypad[0xD] = false;
            break;

          case SDLK_a:
            chip8->keypad[0x7] = false;
            break;
          case SDLK_s:
            chip8->keypad[0x8] = false;
            break;
          case SDLK_d:
            chip8->keypad[0x9] = false;
            break;
          case SDLK_f:
            chip8->keypad[0xE] = false;
            break;

          case SDLK_z:
            chip8->keypad[0xA] = false;
            break;
          case SDLK_x:
            chip8->keypad[0x0] = false;
            break;
          case SDLK_c:
            chip8->keypad[0xB] = false;
            break;
          case SDLK_v:
            chip8->keypad[0xF] = false;
            break;

          default:
            break;
        }
        break;

      default:
        break;
    }
  }
}

uint8_t read_byte(uint8_t *memory, uint16_t address) {
  address &= (RAM_MAX - 1);
  return memory[address];
}

void write_byte(uint8_t *memory, uint16_t address, uint8_t value) {
  address &= (RAM_MAX - 1);
  memory[address] = value;
}
