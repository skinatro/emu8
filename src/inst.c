#include "inst.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chip8.h"
#include "util.h"

void eraseDisplay(chip8_t *myChip8) {
  puts("Instruction Decoded - 00E0");
  memset(myChip8->pixels, 0,
         sizeof(myChip8->pixels[0]) * (DISPLAY_X * DISPLAY_Y));
}

void returnFromSubroutine(chip8_t *myChip8) {
  puts("Instruction Decoded - 00EE");
  myChip8->program_counter = myChip8->stack[--myChip8->stack_pointer];
}

void jumpToAddress(chip8_t *myChip8, uint16_t NNN) {
  puts("Instruction Decoded - 1NNN");
  myChip8->program_counter = NNN;
  printf("Jumped to %x\n", NNN);
}

void callSubroutineAtAddress(chip8_t *myChip8, uint16_t NNN) {
  puts("Instruction Decoded - 2NNN");
  myChip8->stack[myChip8->stack_pointer++] = myChip8->program_counter;
  myChip8->program_counter = NNN;
}

void skipIfRegisterEqualsValue(chip8_t *myChip8, uint8_t X, uint8_t KK) {
  puts("Instruction Decoded - 3XKK");
  if (myChip8->registers[X] == KK) {
    myChip8->program_counter += 2;
  }
}
void skipIfRegisterNotEqualsValue(chip8_t *myChip8, uint8_t X, uint8_t KK) {
  puts("Instruction Decoded - 4XKK");
  if (myChip8->registers[X] != KK) {
    myChip8->program_counter += 2;
  }
}

void skipIfRegistersEqual(chip8_t *myChip8, uint8_t X, uint8_t Y) {
  puts("Instruction Decoded - 5XY0");
  if (myChip8->registers[X] == myChip8->registers[Y]) {
    myChip8->program_counter += 2;
  }
}

void loadValueIntoRegister(chip8_t *myChip8, uint8_t X, uint8_t KK) {
  puts("Instruction Decoded - 6XKK");
  myChip8->registers[X] = KK;
}

void addValueToRegister(chip8_t *myChip8, uint8_t X, uint8_t KK) {
  puts("Instruction Decoded - 7XKK");
  myChip8->registers[X] += KK;
}

void loadRegisterIntoRegister(chip8_t *myChip8, uint8_t X, uint8_t Y) {
  puts("Instruction Decoded - 8XY0");
  myChip8->registers[X] = myChip8->registers[Y];
}

void bitwiseOrRegisters(chip8_t *myChip8, uint8_t X, uint8_t Y) {
  puts("Instruction Decoded - 8XY1");
  myChip8->registers[X] |= myChip8->registers[Y];
  myChip8->registers[0xF] = 0;
}

void bitwiseAndRegisters(chip8_t *myChip8, uint8_t X, uint8_t Y) {
  puts("Instruction Decoded - 8XY2");
  myChip8->registers[X] &= myChip8->registers[Y];
  myChip8->registers[0xF] = 0;
}

void bitwiseXorRegisters(chip8_t *myChip8, uint8_t X, uint8_t Y) {
  puts("Instruction Decoded - 8XY3");
  myChip8->registers[X] ^= myChip8->registers[Y];
  myChip8->registers[0xF] = 0;
}

void addRegistersWithCarry(chip8_t *myChip8, uint8_t X, uint8_t Y) {
  puts("Instruction Decoded - 8XY4");
  uint16_t sum = (myChip8->registers[X] + myChip8->registers[Y]);
  myChip8->registers[X] = sum & 0xFF;
  myChip8->registers[0xF] = (sum > 255);
}

void subtractRegisters(chip8_t *myChip8, uint8_t X, uint8_t Y) {
  puts("Instruction Decoded - 8XY5");
  uint8_t carry, sum;
  sum = myChip8->registers[X] - myChip8->registers[Y];
  carry = (myChip8->registers[X] >= myChip8->registers[Y]);
  myChip8->registers[X] = sum;
  myChip8->registers[0xF] = carry;
}

void shiftRegisterRight(chip8_t *myChip8, uint8_t X, uint8_t Y) {
  puts("Instruction Decoded - 8XY6");
  myChip8->registers[X] = myChip8->registers[Y];
  uint8_t lsb = (myChip8->registers[X] & 0x01);
  myChip8->registers[X] >>= 1;
  myChip8->registers[0xF] = lsb;
}

void subtractRegistersNegated(chip8_t *myChip8, uint8_t X, uint8_t Y) {
  puts("Instruction Decoded - 8XY7");
  uint8_t carry, diff;
  diff = myChip8->registers[Y] - myChip8->registers[X];
  carry = (myChip8->registers[Y] >= myChip8->registers[X]);
  myChip8->registers[X] = diff;
  myChip8->registers[0xF] = carry;
}

void shiftRegisterLeft(chip8_t *myChip8, uint8_t X, uint8_t Y) {
  puts("Instruction Decoded - 8XYE");
  myChip8->registers[X] = myChip8->registers[Y];
  uint8_t msb = (myChip8->registers[X] & 0x80) >> 7;
  myChip8->registers[X] <<= 1;
  myChip8->registers[0xF] = msb;
}

void skipIfRegistersNotEqual(chip8_t *myChip8, uint8_t X, uint8_t Y) {
  puts("Instruction Decoded - 9XY0");
  if (myChip8->registers[X] != myChip8->registers[Y])
    myChip8->program_counter += 2;
}

void loadIndexRegister(chip8_t *myChip8, uint16_t NNN) {
  puts("Instruction Decoded - ANNN");
  myChip8->index_register = NNN;
}

void jumpToAddressPlusZeroRegister(chip8_t *myChip8, uint16_t NNN) {
  puts("Instruction Decoded - BNNN");
  myChip8->program_counter = myChip8->registers[0] + NNN;
}

void generateRandomNumber(chip8_t *myChip8, uint8_t X, uint8_t KK) {
  puts("Instruction Decoded - CXKK");
  myChip8->registers[X] = (rand() % 256) & KK;
}

void drawSprite(chip8_t *myChip8, uint8_t X, uint8_t Y, uint8_t N) {
  puts("Instruction Decoded - DXYN");
  int wrap = 0;
  uint8_t x = myChip8->registers[X] % DISPLAY_X;
  uint8_t y = myChip8->registers[Y] % DISPLAY_Y;
  myChip8->registers[0xF] = 0;

  for (int row = 0; row < N; row++) {
    if (!wrap && y + row >= DISPLAY_Y) break;

    uint8_t sprite = myChip8->memory[myChip8->index_register + row];

    for (int col = 0; col < 8; col++) {
      if (!wrap && x + col >= DISPLAY_X) break;

      if ((sprite & 0x80) > 0) {
        uint8_t pixel_x = wrap ? (x + col) % DISPLAY_X : x + col;
        uint8_t pixel_y = wrap ? (y + row) % DISPLAY_Y : y + row;

        if (myChip8->pixels[pixel_x + pixel_y * DISPLAY_X] == 1) {
          myChip8->registers[0xF] = 1;
        }
        myChip8->pixels[pixel_x + pixel_y * DISPLAY_X] ^= 1;
      }
      sprite <<= 1;
    }
  }
  myChip8->draw_flag = 1;
}

void skipIfKeyPressed(chip8_t *myChip8, uint8_t X) {
  puts("Instruction Decoded - EX9E");
  if (myChip8->keypad[myChip8->registers[X] & 0x0F]) {
    myChip8->program_counter += 2;
  }
}

void skipIfKeyNotPressed(chip8_t *myChip8, uint8_t X) {
  puts("Instruction Decoded - EXA1");
  if (!myChip8->keypad[myChip8->registers[X] & 0x0F]) {
    myChip8->program_counter += 2;
  }
}

void loadDelayTimer(chip8_t *myChip8, uint8_t X) {
  puts("Instruction Decoded - FX07");
  myChip8->registers[X] = myChip8->delay_timer;
}

void waitForKeyPress(chip8_t *myChip8, uint8_t X) {
  puts("Instruction Decoded - FX0A");
  for (size_t i = 0; i < sizeof(myChip8->keypad); i++) {
    if (myChip8->keypad[i]) {
      myChip8->registers[X] = i;
      return;
    }
  }
  myChip8->program_counter -= 2;
}

void setDelayTimer(chip8_t *myChip8, uint8_t X) {
  puts("Instruction Decoded - FX15");
  myChip8->delay_timer = myChip8->registers[X];
}
void setSoundTimer(chip8_t *myChip8, uint8_t X) {
  puts("Instruction Decoded - FX18");
  myChip8->sound_timer = myChip8->registers[X];
}
void addRegisterToIndex(chip8_t *myChip8, uint8_t X) {
  puts("Instruction Decoded - FX1E");
  myChip8->index_register += myChip8->registers[X];
}

void loadFontCharacter(chip8_t *myChip8, uint8_t X) {
  puts("Instruction Decoded - FX29");
  myChip8->index_register = FONT_START + 5 * (myChip8->registers[X] & 0x0F);
}

void storeBCDRepresentation(chip8_t *myChip8, uint8_t X) {
  puts("Instruction Decoded - FX33");
  uint8_t value = myChip8->registers[X];
  uint16_t address = myChip8->index_register;

  write_byte(myChip8->memory, address, value / 100);
  write_byte(myChip8->memory, address + 1, (value / 10) % 10);
  write_byte(myChip8->memory, address + 2, value % 10);
}

void dumpRegisters(chip8_t *myChip8, uint8_t X) {
  puts("Instruction Decoded - FX55");
  for (uint8_t i = 0; i <= X; i++) {
    uint16_t address = myChip8->index_register + i;
    write_byte(myChip8->memory, address, myChip8->registers[i]);
  }
  myChip8->index_register += X + 1;
}

void loadRegisters(chip8_t *myChip8, uint8_t X) {
  puts("Instruction Decoded - FX65");
  for (uint8_t i = 0; i <= X; i++) {
    uint16_t address = myChip8->index_register + i;
    myChip8->registers[i] = read_byte(myChip8->memory, address);
  }
  myChip8->index_register += X + 1;
}
