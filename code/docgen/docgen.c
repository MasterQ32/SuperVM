#include "mnemonics.h"

#include <stdio.h>
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
    
		fprintf(target, "| Property  | Configuration |\n");
		fprintf(target, "|-----------|---------------|\n");
		fprintf(target, "| Execution | %13s |\n", execution);
		fprintf(target, "| Input 0   | %13s |\n", input0);
		fprintf(target, "| Input 1   | %13s |\n", input1);
		fprintf(target, "| Result    | %13s |\n", result);
		fprintf(target, "| Set Flags | %13s |\n", (mnemonics[i].instr.flags ? "yes" : "no") );
		
    fprintf(target, "\n");
  }
  

  return 0;
}