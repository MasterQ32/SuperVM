#pragma once

#include "vm.h"

struct mnemonic
{
	const char *name;
	const char *description;
	instruction_t instr;
};

typedef struct mnemonic mnemonic_t;

extern const mnemonic_t mnemonics[];