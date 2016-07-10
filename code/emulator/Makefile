
CC     = gcc
LD     = gcc
LEX    = flex
TMPDIR = ../obj/
OUTDIR = ../bin/

FLAGS += -g -I../common/

all: emulator

emulator: \
					emulator.o visualmode.o textmode.o \
					exp.o mnemonics.o disassembler.o vm.o \
					devices/timer.o devices/serial.o devices/gpu.o
	$(LD) $(FLAGS) -o $(addprefix $(OUTDIR), $@) $(addprefix $(TMPDIR), $^) -lSDL -lncurses -lpanel -lform

%.o: %.c
	$(CC) $(FLAGS) -c -o $(addprefix $(TMPDIR), $@) $^

tokens.c: tokens.y
	$(LEX) -o $@ --yylineno $^
	
include ../common/common.mk