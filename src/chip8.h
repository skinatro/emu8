#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

typedef enum {
  RAM_MAX = 4096,     // Maximum size of the memory
  DISPLAY_X = 64,     // Width of the display
  DISPLAY_Y = 32,     // Height of the display
  STACK_MAX = 16,     // Maximum size of the stack
  REGISTERS = 16,     // Number of registers
  RAM_START = 0x200,  // ROM is loaded into memory here
  FONT_START = 0x50,  // Fontset is loaded here
  FONT_MAX = 80,      // Size of fontset
} CHIP8;

typedef struct {
  uint8_t memory[RAM_MAX];                // Memory of the CHIP-8 system
  uint8_t pixels[DISPLAY_X * DISPLAY_Y];  // Pixels of the display
  uint16_t program_counter;               // Program counter
  uint16_t index_register;                // Index register
  uint16_t stack[STACK_MAX];              // Stack
  uint16_t stack_pointer;                 // Stack pointer
  uint8_t delay_timer;                    // Delay timer
  uint8_t sound_timer;                    // Sound timer
  uint8_t registers[REGISTERS];           // Registers
  uint8_t keypad[16];                     // Keyboard state
  uint16_t instruction;
  uint8_t running_flag;  // Flag to set running condition of the emulator
  uint8_t draw_flag;     // To see if the screen is modified and whether to draw
                         // the new frame
} chip8_t;

typedef struct {
  uint16_t opcode;
  uint16_t NNN;
  uint8_t KK;
  uint8_t N;
  uint8_t X;
  uint8_t Y;
} inst_t;

extern uint8_t fontset[FONT_MAX];  // extern extends visibility to other

void initialize(chip8_t *);
void cpu_cycle(chip8_t *);
void decode(inst_t *, chip8_t *);
void fetch(chip8_t *);

#endif
