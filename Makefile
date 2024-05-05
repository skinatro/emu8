CC = gcc
CFLAGS = -ggdb -g -Wall -Wextra -Werror -Og -march=native -pipe -std=c11
TARGET_EXEC := emu8

SRC_DIR := ./src
BUILD_DIR := ./build

files:= main.c chip8.c util.c inst.c
objects := $(patsubst %.c,$(BUILD_DIR)/%.o,$(files))

$(info $(objects))

.PHONY: clean all directories rebuild

all: directories $(TARGET_EXEC)

$(TARGET_EXEC): $(objects)
	$(CC) -ggdb $(objects) -o $(TARGET_EXEC) `sdl2-config --cflags --libs` -lm

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ && cd $(SRC_DIR)  # Explicit rule, set working directory

directories:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET_EXEC)

rebuild: clean all
