
#include <stdbool.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include "tokens.h"
#include "../vm.h"
#include "../mnemonics.h"

struct llist
{
	char name[128];
	uint32_t value;
	struct llist *next;
};

typedef struct llist llist_t;

void list_insert(llist_t **list, const char *name, uint32_t value)
{
	llist_t *item = malloc(sizeof(struct llist));
	strcpy(item->name, name);
	item->value = value;
	item->next = *list;
	*list = item;
}

uint32_t list_find(llist_t **list, const char *name)
{
	for(llist_t *it = *list; it != NULL; it = it->next)
	{
		if(strcmp(it->name, name) == 0) {
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

FILE *output;

int listSymbols = 0;

// current token
int tok;

void apply_modifiers(instruction_t *i)
{
	while(tok == TOK_MOD)
	{
		printf("modifier, ");
		tok = yylex();
	}
}

// line format:
// TOK_LABEL? TOK_MOD* TOK_MNEMONIC TOK_MOD* ()? TOK_MOD* TOK_NEWLINE
void assemble()
{
	uint32_t index = 0;
	llist_t *labels = NULL;
	llist_t *patches = NULL;
	while(true)
	{
		tok = yylex();
		if(tok == TOK_EOF) {
			break;
		}
		if(tok == TOK_LABEL)
		{
			int len = strlen(yytext);
			yytext[len - 1] = '\0'; // remove colon
		
			list_insert(&labels, yytext, index);
			
			tok = yylex();
		}
		if(tok != TOK_NEWLINE)
		{
			instruction_t current = { 0 };
		
			apply_modifiers(&current);
			
			if(tok != TOK_MNEMONIC)
			{
				fprintf(stderr, "Expected mnemonic!\n");
				exit(1);
			}
			
			// Gather mnemonic to generate base instruction
			for(int i = 0; ; i++)
			{
				if(mnemonics[i].name == NULL) {
					fprintf(stderr, "Invalid mnemonic: %s\n", yytext);
					exit(1);
				}
				else if(strcmp(mnemonics[i].name, yytext) == 0) {
					current = mnemonics[i].instr;
					break;
				}
			}
			
			tok = yylex();
			
			apply_modifiers(&current);
			
			if(tok != TOK_NEWLINE)
			{
				switch(tok)
				{
					case TOK_INT:
						current.argument = (uint32_t)atoi(yytext);
						break;
					case TOK_HEX:
						current.argument = (uint32_t)strtol(yytext, NULL, 16);
						break;
					case TOK_CHAR:
						current.argument = (uint32_t)yytext[1];
						break;
					case TOK_REFERENCE:
					{
						// insert patch here for deferred argument modification
						// (yytext + 1) removes the leading @
						list_insert(&patches, yytext + 1, index);
						break;
					}
					default: 
						fprintf(stderr, "Invalid token detected: %d\n", tok);
						exit(1);
				}
				tok = yylex();
				apply_modifiers(&current);
			}
			if(tok != TOK_NEWLINE)
			{
				fprintf(stderr, "Invalid token detected: %d\n", tok);
				exit(1);
			}
			
			// write command:
			fwrite(&current, sizeof(instruction_t), 1, output);
			
			// Increase command index by one
			index += 1;
		}
		printf("NEXT\n");
	}
	
	if(listSymbols)
	{
		printf("Symbols:\n");
		for(llist_t *it = labels; it != NULL; it = it->next)
		{
			printf("%s@%d\n", it->name, it->value);
		}
	}
	
	printf("Patches:\n");
	for(llist_t *it = patches; it != NULL; it = it->next)
	{
		uint32_t target = list_find(&labels, it->name);
		
		// Seek to the target address
		fseek(output, sizeof(instruction_t) * it->value + 4, SEEK_SET);
		fwrite(&target, 1, sizeof(uint32_t), output);
		
		printf("%d -> %d (%s)\n", it->value, target, it->name);
	}
}

int main(int argc, char **argv)
{
	output = stdout;

	int c;
	while ((c = getopt(argc, argv, "o:")) != -1)
	{
		switch (c)
		{
		case 'o':
		{			
			FILE *f = fopen(optarg, "wb");
			if(f == NULL) {
				fprintf(stderr, "%f not found.\n", optarg);
				abort();
			}
			if(output != stdout) {
				fclose(output);
			}
			output = f;
			break;
		}
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
	for (int index = optind; index < argc; index++)
	{
		FILE *f = fopen(argv[index], "r");
		if(f == NULL) {
			fprintf(stderr, "%f not found.\n", optarg);
			abort();
		}
		FILE *p = yyget_in();
		if(p != NULL && p != stdin) {
			fclose(p);
		}
		yyset_in(f);
		break;
	}

	assemble();
	
	FILE *p = yyget_in();
	if(p != NULL && p != stdin) {
		fclose(p);
	}
	
	return 0;
}