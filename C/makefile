CC=gcc
SRC=src/*.c
INCLUDE=-I include
FLAGS=-O3 -Wall -Werror -march=native -mtune=native
BIN=bin/rpizw-weather-station

.PHONY: clean all

all:
	mkdir -p bin
	$(CC) -o $(BIN) $(INCLUDE) $(FLAGS) $(SRC)

asm:
	$(CC) -S $(INCLUDE) $(FLAGS) $(SRC)
	
clean:	
	rm -rf $(BIN) *.s
