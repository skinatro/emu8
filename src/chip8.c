#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inst.h"
#include "util.h"

chip8_t myChip8;

uint8_t fontset[FONT_MAX] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

void initialize(chip8_t *myChip8) {
  myChip8->program_counter = 0x200;  // Set pc to start
  myChip8->index_register = 0;       // Initialize index register
  myChip8->stack_pointer = 0;        // Initialize stack pointer
  myChip8->delay_timer = 0;          // Initialize delay timer
  myChip8->sound_timer = 0;          // Initialize sound timer
  myChip8->draw_flag = 1;            // Set screen state
  myChip8->running_flag = 1;         // Set machine execution state to running
  myChip8->instruction = 0;

  memset(myChip8->registers, 0, sizeof(myChip8->registers));
  memset(myChip8->memory, 0, sizeof(myChip8->memory));
  memset(myChip8->stack, 0, sizeof(myChip8->stack));

  memset(myChip8->keypad, 0, sizeof(myChip8->keypad));

  puts("Initialized the Chip8");

  memset(myChip8->pixels, 0, sizeof(myChip8->pixels));

  puts("Cleared the screen");

  memcpy(&myChip8->memory[FONT_START], fontset, FONT_MAX);
  puts("Populated memory with fontset");
}

void cpu_cycle(chip8_t *myChip8) {
  inst_t inst;

  if (myChip8->program_counter >= RAM_MAX) {
    myChip8->running_flag = 0;
    return;
  }

  fetch(myChip8);
  inst.opcode = myChip8->instruction;

  myChip8->program_counter += 2;

  decode(&inst, myChip8);
}

void decode(inst_t *instruction, chip8_t *myChip8) {
  printf("Decoding Opcode: %x \n", instruction->opcode);

  instruction->NNN = instruction->opcode & 0x0FFF;
  instruction->KK = instruction->opcode & 0x00FF;
  instruction->N = instruction->opcode & 0x000F;
  instruction->X = (instruction->opcode & 0x0F00) >> 8;
  instruction->Y = (instruction->opcode & 0x00F0) >> 4;

  switch ((instruction->opcode & 0xF000) >> 12) {
    case 0x0:
      switch (instruction->opcode & 0x0FFF) {
        case 0xE0:
          eraseDisplay(myChip8);
          break;

        case 0xEE:
          returnFromSubroutine(myChip8);
          break;

        default:
          printf("Unhandled Opcode %d\n", instruction->opcode);
          break;
      }
      break;
    case 0x1:
      jumpToAddress(myChip8, instruction->NNN);
      break;
    case 0x2:
      callSubroutineAtAddress(myChip8, instruction->NNN);
      break;
    case 0x3:
      skipIfRegisterEqualsValue(myChip8, instruction->X, instruction->KK);
      break;
    case 0x4:
      skipIfRegisterNotEqualsValue(myChip8, instruction->X, instruction->KK);
      break;
    case 0x5:
      if ((instruction->opcode & 0x000F) == 0) {
        skipIfRegistersEqual(myChip8, instruction->X, instruction->Y);
      }
      break;
    case 0x6:
      loadValueIntoRegister(myChip8, instruction->X, instruction->KK);

      break;
    case 0x7:
      addValueToRegister(myChip8, instruction->X, instruction->KK);

      break;
    case 0x8:
      switch (instruction->opcode & 0x000F) {
        case 0x0:
          loadRegisterIntoRegister(myChip8, instruction->X, instruction->Y);

          break;

        case 0x1:
          bitwiseOrRegisters(myChip8, instruction->X, instruction->Y);

          break;

        case 0x2:
          bitwiseAndRegisters(myChip8, instruction->X, instruction->Y);

          break;

        case 0x3:
          bitwiseXorRegisters(myChip8, instruction->X, instruction->Y);

          break;

        case 0x4:
          addRegistersWithCarry(myChip8, instruction->X, instruction->Y);

          break;

        case 0x5:
          subtractRegisters(myChip8, instruction->X, instruction->Y);

          break;

        case 0x6:
          shiftRegisterRight(myChip8, instruction->X, instruction->Y);

          break;

        case 0x7:
          subtractRegistersNegated(myChip8, instruction->X, instruction->Y);
          break;

        case 0xE:
          shiftRegisterLeft(myChip8, instruction->X, instruction->Y);

          break;

        default:
          printf("Unhandled Opcode %d\n", instruction->opcode);

          break;
      }
      break;
    case 0x9:
      if ((instruction->opcode & 0x000F) == 0) {
        skipIfRegistersNotEqual(myChip8, instruction->X, instruction->Y);
      }
      break;
    case 0xA:
      loadIndexRegister(myChip8, instruction->NNN);

      break;
    case 0xB:
      jumpToAddressPlusZeroRegister(myChip8, instruction->NNN);
      break;
    case 0xC:
      generateRandomNumber(myChip8, instruction->X, instruction->KK);

      break;
    case 0xD:
      drawSprite(myChip8, instruction->X, instruction->Y, instruction->N);

      break;
    case 0xE:
      switch (instruction->opcode & 0x00FF) {
        case 0x9E:
          skipIfKeyPressed(myChip8, instruction->X);
          break;

        case 0xA1:
          skipIfKeyNotPressed(myChip8, instruction->X);
          break;

        default:
          printf("Unhandled Opcode %d\n", instruction->opcode);

          break;
      }
      break;
    case 0xF:
      switch (instruction->opcode & 0x00FF) {
        case 0x07:
          loadDelayTimer(myChip8, instruction->X);

          break;

        case 0x0A:
          waitForKeyPress(myChip8, instruction->X);

          break;

        case 0x15:
          setDelayTimer(myChip8, instruction->X);

          break;

        case 0x18:
          setSoundTimer(myChip8, instruction->X);

          break;

        case 0x1E:
          addRegisterToIndex(myChip8, instruction->X);

          break;

        case 0x29:
          loadFontCharacter(myChip8, instruction->X);

          break;

        case 0x33:
          storeBCDRepresentation(myChip8, instruction->X);

          break;

        case 0x55:
          dumpRegisters(myChip8, instruction->X);

          break;

        case 0x65:
          loadRegisters(myChip8, instruction->X);

          break;

        default:
          printf("Unhandled Opcode %d\n", instruction->opcode);

          break;
      }
      break;

    default:
      printf("Unhandled Opcode %d\n", instruction->opcode);

      break;
  }
}

void fetch(chip8_t *myChip8) {
  uint8_t higher_byte = read_byte(myChip8->memory, myChip8->program_counter);
  uint8_t lower_byte = read_byte(myChip8->memory, myChip8->program_counter + 1);

  myChip8->instruction = (higher_byte << 8) | lower_byte;

  printf("The opcode @ %x is %x\n", myChip8->program_counter,
         myChip8->instruction);
  printf("PC: addr %x\n", myChip8->program_counter);
}
