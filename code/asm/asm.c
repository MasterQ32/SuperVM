#include "vm.h"
#include "mnemonics.h"
#include "disassembler.h"
#include "exp.h"
#include "sectionlist.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#include <getopt.h>

bool process(const char *fileName);

sectlist_t *currentSection = NULL;

llist_t *labels = NULL;
llist_t *variables = NULL;
llist_t *patches = NULL;

FILE *output = NULL;

void new_section(const char *name, uint32_t base)
{
	sectlist_t *next = currentSection;
	
	currentSection = malloc(sizeof(sectlist_t));
	memset(currentSection, 0x00, sizeof(sectlist_t));
	strcpy(currentSection->section.name, name);
	currentSection->section.base = base;
	currentSection->section.start = ftell(output);
	
	currentSection->next = next;
}

int main(int argc, char **argv)
{	
	int c;
	while ((c = getopt(argc, argv, "")) != -1)
	{
		switch (c)
		{
		case '?':
			if (optopt == 'o')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%02x'.\n", optopt);
			return 1;
		default:
			abort();
		}
	}
	
  if(optind >= argc) {
    fprintf(stderr, "%s: No input files.\n", argv[0]);
    exit(1);
  }
  
	output = fopen("a.exp", "wb");
	if(output == NULL) {
		fprintf(stderr, "%s: Could not open output file %s.", argv[0]);
	}
	
	expfile_t header = {
		EXP_MAGIC,
		1,0,
		0, 					// Number of metadata entries
		0xFFFFFFFF, // Number of sections
		0,					// File pointer of first metadata entry
		0xFFFFFFFF, // File pointer of first section definition;
	};
	
	fwrite(&header, sizeof(expfile_t), 1, output);
	
	// Create first section after writing the header.
	new_section("initial", 0x00000000);
	
	for (int index = optind; index < argc; index++)
	{
		fprintf(stderr, "Assembling %s\n", argv[index]);
		process(argv[index]);
		fprintf(stderr, "? %d\n", currentSection->section.length);
	}
	
	header.posSections = ftell(output);
	header.numSections = 0;
	
	for(sectlist_t *s = currentSection; s != NULL; s = s->next)
	{
		if(s->section.length == 0)
			continue;
		header.numSections++;
		fwrite(&s->section, sizeof(expsection_t), 1, output);
	}
	
	fseek(output, 0, SEEK_SET);
	fwrite(&header, sizeof(expfile_t), 1, output);
	
	printf("Patches:\n");
	for(llist_t *it = patches; it != NULL; it = it->next)
	{
		uint32_t target = addrToInstr(list_find(&labels, it->name));
		if(listFound == 0)
		{
			fprintf(stderr, "Could not find label %s.\n", it->name);
			exit(1);
		}
		
		// Seek to the target address
		fseek(output, it->value, SEEK_SET);
		fwrite(&target, 1, sizeof(uint32_t), output);
		
		printf("%d -> %d (%s)\n", it->value, target, it->name);
	}
	
	fflush(output);
	fclose(output);
	
	return 0;
}

void list_insert(llist_t **list, const char *name, uint32_t value)
{
	llist_t *item = malloc(sizeof(struct llist));
	strcpy(item->name, name);
	item->value = value;
	item->next = *list;
	*list = item;
}

int listFound = 0;
uint32_t list_find(llist_t **list, const char *name)
{
	listFound = 0;
	for(llist_t *it = *list; it != NULL; it = it->next)
	{
		if(strcmp(it->name, name) == 0) {
			listFound = 1;
			return it->value;
		}
	}
	return -1;
}

void list_free(llist_t **list)
{
	while((*list) != NULL)
	{
		llist_t *next = (*list)->next;
		free(*list);
		(*list) = next;
	}
}