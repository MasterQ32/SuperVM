
CC = gcc
CFLAGS=-O3

AS=./bin/svmas
LN=./bin/svmln

all: svmln expdump emulator svmas

.PHONY: as clean run

svmln: svmln.c
	$(CC) -g -o bin/$@ $^ $(CFLAGS)

expdump: expdump.c mnemonics.c disassembler.c
	$(CC) -g -o bin/$@ $^ $(CFLAGS)

emulator: emulator.c vm.c
	$(CC) -g -o bin/$@ $^ -lSDL $(CFLAGS)

svmas:
	$(MAKE) -C as

%.bin: %.asm
	$(AS) -o $@ $^

%.exp: %.bin
	$(LN) -o $@ $(addprefix -c ,$^) -d input.dat -E 4096

clean:
	rm *.bin
