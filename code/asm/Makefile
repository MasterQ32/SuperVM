
CC     = gcc
LD     = gcc
LEX    = flex
TMPDIR = ../obj/
OUTDIR = ../bin/

FLAGS += -g -I../common/

all: asm

asm: tokens.o asm.o mnemonics.o disassembler.o
	$(LD) $(FLAGS) -o $(addprefix $(OUTDIR), $@) $(addprefix $(TMPDIR), $^)

%.o: %.c
	$(CC) $(FLAGS) -c -o $(addprefix $(TMPDIR), $@) $^

tokens.c: tokens.y
	$(LEX) -o $@ --yylineno $^
	
include ../common/common.mk