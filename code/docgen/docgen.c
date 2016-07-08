#include <stdio.h>

#include "mnemonics.h"


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
    fprintf(target, "## %s\n", mnemonics[i].name);
    fprintf(target, "%s\n", mnemonics[i].description);
    fprintf(target, "\n");
  }
  

  return 0;
}