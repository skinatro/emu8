#ifndef UTIL_H
#define UTIL_H

#include <SDL2/SDL.h>

#include "chip8.h"

#define IPF 17

#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32
#define WINDOW_SCALE 10
#define RENDERER_R 0
#define RENDERER_G 0
#define RENDERER_B 0
#define RENDERER_A 255

#define RECT_R 255
#define RECT_G 255
#define RECT_B 255
#define RECT_A 255

#define AUDIO_SAMPLING_RATE 44100
#define AUDIO_AMPLITUDE 10000

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} SDLResources;

void InitSDL(SDLResources *);
void cleanup(SDLResources *);
void load_rom(chip8_t *, const char *);
void draw(SDLResources *, chip8_t *, SDL_Rect *);
void handle_input(chip8_t *);

uint8_t read_byte(uint8_t *memory, uint16_t address);
void write_byte(uint8_t *memory, uint16_t address, uint8_t value);

void audio_cb(void *userdata, unsigned short *raw_buf, int bytes);
#endif
