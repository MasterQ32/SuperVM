
CC     = gcc
LD     = gcc
LEX    = flex
TMPDIR = ../obj/
OUTDIR = ../bin/

FLAGS += -I../common/

all: link

link: link.o mnemonics.o disassembler.o
	$(LD) $(FLAGS) -o $(addprefix $(OUTDIR), $@) $(addprefix $(TMPDIR), $^)

%.o: %.c
	$(CC) $(FLAGS) -c -o $(addprefix $(TMPDIR), $@) $^

include ../common/common.mk