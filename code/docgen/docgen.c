#include "mnemonics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  FILE *target = stdout;
  
  
  
  fprintf(target, "# Assembler Mnemonics\n");
  fprintf(target, "\n");

  for(int i = 0; ; i++)
  {
    if(mnemonics[i].name == NULL) {
      break;
    }
		if(mnemonics[i].instr.input0 == VM_INPUT_ARG)
			fprintf(target, "## %s $arg\n", mnemonics[i].name);
    else
			fprintf(target, "## %s\n", mnemonics[i].name);
    fprintf(target, "%s\n", mnemonics[i].description);
    fprintf(target, "\n");
		
		const char *input0 = NULL;
		const char *input1 = NULL;
		const char *result = NULL;
		char execution[64];
		strcpy(execution, "always");
		
		switch(mnemonics[i].instr.input0)
		{
			case VM_INPUT_ZERO: input0 = "zero"; break;
			case VM_INPUT_POP: input0 = "pop"; break;
			case VM_INPUT_PEEK: input0 = "peek"; break;
			case VM_INPUT_ARG: input0 = "argument"; break;
		}
		switch(mnemonics[i].instr.input1)
		{
			case VM_INPUT_ZERO: input1 = "zero"; break;
			case VM_INPUT_POP: input1 = "pop"; break;
			// case VM_INPUT_PEEK: input1 = "peek"; break;
			// case VM_INPUT_ARG: input1 = "argument"; break;
		}
		
		switch(mnemonics[i].instr.output)
		{
			case VM_OUTPUT_DISCARD: result = "discard"; break;
			case VM_OUTPUT_PUSH: result = "push"; break;
			case VM_OUTPUT_JUMP: result = "jump"; break;
			case VM_OUTPUT_JUMPR: result = "jump (relative)"; break;
		}
		
		if(mnemonics[i].instr.execZ != VM_EXEC_X || mnemonics[i].instr.execN != VM_EXEC_X)
		{
			strcpy(execution, "");
			switch(mnemonics[i].instr.execZ)
			{
				case VM_EXEC_X: break;
				case VM_EXEC_0: strcat(execution, "¬z"); break;
				case VM_EXEC_1: strcat(execution, "z"); break;
			}
			if(mnemonics[i].instr.execZ != VM_EXEC_X && mnemonics[i].instr.execN != VM_EXEC_X)
			{
				strcat(execution, " ∧ ");
			}	
			switch(mnemonics[i].instr.execN)
			{
				case VM_EXEC_X: break;
				case VM_EXEC_0: strcat(execution, "¬n"); break;
				case VM_EXEC_1: strcat(execution, "n"); break;
			}
		}
		
		const char *command = NULL;
		switch(mnemonics[i].instr.command)
		{
			case VM_CMD_COPY: command = "copy"; break;
			case VM_CMD_STORE: command = "store"; break;
			case VM_CMD_LOAD: command = "load"; break;  
			case VM_CMD_GET: command = "get"; break;
			case VM_CMD_SET: command = "set"; break;
			case VM_CMD_BPGET: command = "bpget"; break;
			case VM_CMD_BPSET: command = "bpset"; break;
			case VM_CMD_CPGET: command = "cpget"; break;
			case VM_CMD_MATH: command = "math"; break;
			case VM_CMD_SPGET: command = "spget"; break;
			case VM_CMD_SPSET: command = "spset"; break;
			case VM_CMD_SYSCALL: command = "syscall"; break;
			case VM_CMD_HWIO: command = "hwio"; break;
			case VM_CMD_INT: command = "int"; break;
			case VM_CMD_SEI: command = "sei"; break;
			case VM_CMD_CLI: command = "cli"; break;
		}
		char cmd[64];
		sprintf(cmd, "%s(%d)", command, mnemonics[i].instr.cmdinfo);
		
		char input[64];
		strcpy(input, input0);
		strcat(input, ",");
		strcat(input, input1);
    
		fprintf(target, "| Property  | Configuration |\n");
		fprintf(target, "|-----------|---------------|\n");
		fprintf(target, "| Execution | %13s |\n", execution);
		fprintf(target, "| Input 0,1 | %13s |\n", input);
		fprintf(target, "| Command   | %13s |\n", cmd);
		fprintf(target, "| Result    | %13s |\n", result);
		fprintf(target, "| Set Flags | %13s |\n", (mnemonics[i].instr.flags ? "yes" : "no") );
		
    fprintf(target, "\n");
  }
  

  return 0;
}