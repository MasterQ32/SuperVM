#include "mnemonics.h"

const mnemonic_t mnemonics[] =
{
	{
		"nop", "Does nothing.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD }
	},
	{ 
		"push", "Pushes $arg on the stack.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{
		"drop", "Removes the top value from the stack.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD }
	},
	{ 
		"dup", "Duplicates the top value of the stack.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_PEEK, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"jmp","Jumps to the command at index $arg.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_JUMP }
	},
	{ 
		"jmpi", "Pops an index from the stack and jumps to it.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_JUMP } 
	},
	{
		"ret", "Returns from a function. Is exactly the same as `jmpi`, but is ment for returning from a function instead of an indirected jump.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_COPY, 0, VM_FLAG_NO, VM_OUTPUT_JUMP } 
	},
	{ 
		"load8", "Loads a byte (8 bit) from $arg.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_LOAD, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{
		"load16", "Loads a half world (16 bit) from $arg.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_LOAD, 1, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{
		"load32", "Loads a word (32 bit) from $arg.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_LOAD, 2, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"load8i", "Pops an address from the stack and loads the byte (8 bit) located at the popped address.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_LOAD, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{
		"load16i", "Pops an address from the stack and loads the half world (16 bit) located at the popped address.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_LOAD, 1, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{
		"load32i", "Pops an address from the stack and loads the word (32 bit) located at the popped address.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_LOAD, 2, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"store8", "Pops a value and stores it as a byte (8 bit) at $arg.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_STORE, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } 
	},
	{
		"store16", "Pops a value and stores it as a half word (16 bit) at $arg.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_STORE, 1, VM_FLAG_NO, VM_OUTPUT_DISCARD }
	},
	{
		"store32", "Pops a value and stores the word (32 bit) at $arg.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_STORE, 2, VM_FLAG_NO, VM_OUTPUT_DISCARD }
	},
	{ 
		"store8i", "First pops an address from the stack, then a value. Then stores the value as a byte (8 bit) at the popped address.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_STORE, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD }
	},
	{
		"store16i", "First pops an address from the stack, then a value. Then stores the value as a half word (16 bit) at the popped address.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_STORE, 1, VM_FLAG_NO, VM_OUTPUT_DISCARD }
	},
	{
		"store32i", "First pops an address from the stack, then a value. Then stores the value (32 bit) at the popped address.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_STORE, 2, VM_FLAG_NO, VM_OUTPUT_DISCARD }
	},
	{ 
		"get", "", // TODO: Documentation 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_GET, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{ 
		"geti", "", // TODO: Documentation 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_GET, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{ 
		"set", "",  // TODO: Documentation
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_SET, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } 
	},
	{
		"seti", "", // TODO: Documentation
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_SET, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } 
	},
	{ 
		"bpget", "Pushes the current base pointer to the stack.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_BPGET, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{
		"bpset", "Pops a value into the base pointer.", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_BPSET, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } 
	},
	{ 
		"spget", "Pushes the current stack pointer to the stack.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_SPGET, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{
		"spset", "Pops a value into the stack pointer.", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_SPSET, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD }
	},
	{
		"cpget", "Pushes the current code pointer incremented by one. This allows pushing return values for function calls.", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_CPGET, 1, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{ 
		"add", "", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 0, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{ 
		"sub", "", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 1, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"cmp", "", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 1, VM_FLAG_YES, VM_OUTPUT_DISCARD } 
	},
	{ 
		"cmpi", "",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 1, VM_FLAG_YES, VM_OUTPUT_DISCARD } 
	},
	{ 
		"mul", "", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 2, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{ 
		"div", "",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 3, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{ 
		"mod", "",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 4, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{ 
		"neg", "", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_MATH, 15, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"and", "",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 5, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"or", "",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 6, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{ 
		"xor", "", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_POP, VM_CMD_MATH, 7, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"not", "", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_POP, VM_INPUT_ZERO, VM_CMD_MATH, 8, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"rol", "",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 9, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"ror", "",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 10, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{ 
		"asl", "",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 11, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"asr", "",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 12, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"shl", "", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 13, VM_FLAG_NO, VM_OUTPUT_PUSH } 
	},
	{ 
		"shr", "",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_MATH, 14, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"syscall", "",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_SYSCALL, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD }
	},
	{
		"hwio", "", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_HWIO, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD }
	},

	// Interrupts
	{ 
		"int", "Raises the interrupt $arg.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_INT, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } 
	},
	{ 
		"sei", "Sets the interrupt flag and thus enables interrupts.", 
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_SEI, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } 
	},
	{ 
		"cli", "Clears the interrupt flag and thus disables interrupts.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ZERO, VM_INPUT_ZERO, VM_CMD_CLI, 0, VM_FLAG_NO, VM_OUTPUT_DISCARD } 
	},

	// Port IO
	{
		"in", "Reads a word from the port $arg.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_ZERO, VM_CMD_HWIO, 0, VM_FLAG_NO, VM_OUTPUT_PUSH }
	},
	{ 
		"out", "Pops a word and writes it to the port $arg.",
		{ VM_EXEC_X, VM_EXEC_X, VM_INPUT_ARG, VM_INPUT_POP, VM_CMD_HWIO, 1, VM_FLAG_NO, VM_OUTPUT_DISCARD }
	},

	// Stop Marker
	{ NULL, NULL, {0} },
};
