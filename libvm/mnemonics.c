#include "mnemonics.h"

const mnemonic_t mnemonics[] =
{
	{ "nop",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	{ "push",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "drop",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	{ "dup",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_PEEK, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "jmp",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_JUMP } },
	{ "jmpi",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_JUMP } },
	{ "ret",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_JUMP } },
	{ "load",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_LOAD, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "loadi",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_LOAD, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "store",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_STORE, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	{ "storei",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_STORE, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	{ "get",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_GET, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "geti",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_GET, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "set",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_SET, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	{ "seti",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_SET, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	{ "bpget",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_BPGET, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "bpset",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_BPSET, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	{ "spget",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_SPGET, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "spset",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_SPSET, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	{ "cpget",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_CPGET, 1, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "add",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "sub",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 1, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "cmp",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 1, VM_FLAG_YES, VM_OUTPUT_DISCARD } },
	{ "cmpi",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 1, VM_FLAG_YES, VM_OUTPUT_DISCARD } },
	{ "mul",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 2, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "div",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 3, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "mod",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 4, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "and",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 5, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "or",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 6, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "xor",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 7, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "not",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_MATH, 8, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "rol",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 9, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "ror",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 10, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "asl",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 11, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "asr",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 12, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "shl",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 13, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "shr",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 14, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "syscall",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_SYSCALL, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	{ "hwio",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_HWIO, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	
	// Interrupts
	{ "int",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_INT, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	{ "sei",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_SEI, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	{ "cli",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_CLI, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } },

	// Port IO
	{ "in",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_HWIO, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } },
	{ "out",{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_HWIO, 1, VM_FLAG_NO, VM_OUTPUT_DISCARD } },
	

	{ NULL, {0} }, // stop marker
};
