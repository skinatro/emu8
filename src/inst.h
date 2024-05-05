#ifndef INST_H
#define INST_H

#include <stdint.h>

#include "chip8.h"

void eraseDisplay(chip8_t *);  // 00E0

void returnFromSubroutine(chip8_t *);  // 00EE

void jumpToAddress(chip8_t *, uint16_t);  // 1NNN

void callSubroutineAtAddress(chip8_t *, uint16_t);  // 2NNN

void skipIfRegisterEqualsValue(chip8_t *, uint8_t,
                               uint8_t);  // 3XKK

void skipIfRegisterNotEqualsValue(chip8_t *, uint8_t,
                                  uint8_t);  // 4XKK

void skipIfRegistersEqual(chip8_t *, uint8_t,
                          uint8_t);  // 5XY0

void loadValueIntoRegister(chip8_t *, uint8_t,
                           uint8_t);  // 6XKK

void addValueToRegister(chip8_t *, uint8_t, uint8_t);  // 7XKK

void loadRegisterIntoRegister(chip8_t *, uint8_t,
                              uint8_t);  // 8XY0

void bitwiseOrRegisters(chip8_t *, uint8_t, uint8_t);  // 8XY1

void bitwiseAndRegisters(chip8_t *, uint8_t,
                         uint8_t);  // 8XY2

void bitwiseXorRegisters(chip8_t *, uint8_t,
                         uint8_t);  // 8XY3

void addRegistersWithCarry(chip8_t *, uint8_t,
                           uint8_t);  // 8XY4

void subtractRegisters(chip8_t *, uint8_t, uint8_t);  // 8XY5

void shiftRegisterRight(chip8_t *, uint8_t, uint8_t);  // 8XY6

void subtractRegistersNegated(chip8_t *, uint8_t,
                              uint8_t);  // 8XY7

void shiftRegisterLeft(chip8_t *, uint8_t, uint8_t);  // 8XYE

void skipIfRegistersNotEqual(chip8_t *, uint8_t,
                             uint8_t);  // 9XY0

void loadIndexRegister(chip8_t *, uint16_t);  // ANNN

void jumpToAddressPlusZeroRegister(chip8_t *,
                                   uint16_t);  // BNNN

void generateRandomNumber(chip8_t *, uint8_t,
                          uint8_t);  // CXKK

void drawSprite(chip8_t *, uint8_t, uint8_t,
                uint8_t);  // DXYN

void skipIfKeyPressed(chip8_t *, uint8_t);  // EX9E

void skipIfKeyNotPressed(chip8_t *, uint8_t);  // EXA1

void loadDelayTimer(chip8_t *, uint8_t);  // FX07

void waitForKeyPress(chip8_t *, uint8_t);  // FX0A

void setDelayTimer(chip8_t *, uint8_t);  // FX15

void setSoundTimer(chip8_t *, uint8_t);  // FX18

void addRegisterToIndex(chip8_t *, uint8_t);  // FX1E

void loadFontCharacter(chip8_t *, uint8_t);  // FX29

void storeBCDRepresentation(chip8_t *, uint8_t);  // FX33

void dumpRegisters(chip8_t *, uint8_t);  // FX55

void loadRegisters(chip8_t *, uint8_t);  // FX65

#endif
