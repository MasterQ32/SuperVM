
CC     = gcc
LD     = gcc
LEX    = flex
TMPDIR = ../obj/
OUTDIR = ../bin/

FLAGS += -I../common/

all: emulator

emulator: emulator.o mnemonics.o disassembler.o vm.o devices/timer.o
	$(LD) $(FLAGS) -o $(addprefix $(OUTDIR), $@) $(addprefix $(TMPDIR), $^) -lSDL

%.o: %.c
	$(CC) $(FLAGS) -c -o $(addprefix $(TMPDIR), $@) $^

tokens.c: tokens.y
	$(LEX) -o $@ --yylineno $^
	
include ../common/common.mk