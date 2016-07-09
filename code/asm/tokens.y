%{
#include <stdbool.h>
#include <stdarg.h>
#define RETURN(x,y) /* fprintf(stderr, "{" #x "}" y ); */ return x

#define TOK_INVALID  -1
#define TOK_EOF       0
#define TOK_NUM       1
#define TOK_LABEL     3
#define TOK_MNEMONIC  4
#define TOK_MOD       5
#define TOK_NEWLINE   6
#define TOK_CHAR      7
#define TOK_REFERENCE 8
#define TOK_VAR       10
#define TOK_STR       11
#define TOK_INCDIR    12
#define TOK_ORGDIR    13
#define TOK_VARDIR    14
#define TOK_ALNDIR    15

%}
comment			;[^\n]*
whitespace	[ \t]
newline     [\n]
digit				[0-9]
hexdigit    [0-9a-fA-F]
intnum			-?{digit}+
hexnum			"0x"{hexdigit}+
binnum      "0b"[01]+
number      {intnum}|{hexnum}|{binnum}
mnemonic    [a-zA-Z0-9]+
labelname   [a-zA-Z0-9_]+
label       ^{labelname}":"
mod         \[[^\]]+\]
character   "'""\\"?."'"
ref         @{labelname}
variable    \${labelname}
string      L?\"(\\.|[^\\"])*\"
%%
{comment}				;
{number} 				RETURN(TOK_NUM,);
{whitespace}		;
{mnemonic}      RETURN(TOK_MNEMONIC,);
{label}         RETURN(TOK_LABEL,); 
{mod}           RETURN(TOK_MOD,);
{character}     RETURN(TOK_CHAR, );
{ref}           RETURN(TOK_REFERENCE, );
{newline}       RETURN(TOK_NEWLINE, "\n");
^:INCLUDE        RETURN(TOK_INCDIR, );
^:ALIGN          RETURN(TOK_ALNDIR, );
^:ORIGIN         RETURN(TOK_ORGDIR, );
^:VAR            RETURN(TOK_VARDIR, );
{variable}      RETURN(TOK_VAR, );
{string}        RETURN(TOK_STR, );
.               RETURN(TOK_INVALID,);
%%
// This is the end of the file
int yywrap() { return 1; }

#include "vm.h"
#include "mnemonics.h"
#include "sectionlist.h"
#include <stdlib.h>

extern FILE *output;

static const char * lastError = NULL;

static void error(const char *currentFileName, const char *msg, ...)
{
	fprintf(stderr, "%s:%d: Error: ", currentFileName, yylineno);
	
  va_list args;
  va_start (args, msg);
  vfprintf (stderr, msg, args);
  va_end (args);
	
	fprintf(stderr, "\n");
	
	exit(1);
}

static void warning(const char *currentFileName, const char *msg, ...)
{
	fprintf(stderr, "%s:%d: Warning: ", currentFileName, yylineno);
	
  va_list args;
  va_start (args, msg);
  vfprintf (stderr, msg, args);
  va_end (args);
	
	fprintf(stderr, "\n");
}

void assert_tok(const char *currentFileName, int type)
{
	int tok;
	if((tok = yylex()) != type) {
		error(currentFileName, "Unexpected symbol or token '%s'(%d)", yytext, tok);
	}
}

char *strdup(const char *str)
{
	char *res = malloc(strlen(str) + 1);
	strcpy(res, str);
	return res;
}

char *makeIncludeRelative(const char *base, const char *rel)
{
	// absolute path handling
	if(rel[0] == '/')
		return strdup(rel);
	int blen = strlen(base);
	int rlen = strlen(rel);
	char *result = malloc(blen + rlen + 1);
	strcpy(result, "");
	
	char *start = strrchr(base, '/');
	if(start != NULL)
		memcpy(result, base, start - base + 1);
	return strcat(result, rel);
}


void apply_modifier(instruction_t *i, const char *mod);

void apply_modifiers(int *tok, instruction_t *i)
{
	while(*tok == TOK_MOD)
	{
		char *s = yytext + 1;
		int len = strlen(s);
		s[len - 1] = 0;
		apply_modifier(i, s);
		*tok = yylex();
	}
}

uint32_t getNum(const char *text)
{
	if(text[1] == 'x')
		return (uint32_t)strtol(text, NULL, 16);
	else if(text[1] == 'b')
		return (uint32_t)strtol(text + 2, NULL, 2);
	else
		return (uint32_t)atoi(yytext);
}

void read_instruction(const char *fileName, int tok)
{
	if(tok == TOK_LABEL)
	{
		int len = strlen(yytext);
		yytext[len - 1] = '\0'; // remove colon

		uint32_t addr = currentSection->section.base + currentSection->section.length;
		
		if((addr & 0xFFFFFFF8) != addr)
		{
			warning(fileName, "Label '%s' is misaligned.", yytext);
		}
		list_insert(&labels, yytext, addr);
		
		tok = yylex();
	}
	if(tok == TOK_NEWLINE)
		return;
	instruction_t current = { 0 };

	apply_modifiers(&tok, &current);
	
	if(tok != TOK_MNEMONIC)
	{
		error(fileName, "Expected mnemonic, got '%s' instead.", yytext);
		exit(1);
	}
	
	// Gather mnemonic to generate base instruction
	for(int i = 0; ; i++)
	{
		if(mnemonics[i].name == NULL) {
			error(fileName, "Invalid mnemonic: '%s'", yytext);
			exit(1);
		}
		else if(strcmp(mnemonics[i].name, yytext) == 0) {
			// Copy instruction here, but account for
			// already applied modificatiors by 
			// only copying when values still zero.
		
			#define COPY_IF(prop) \
				if(!current.prop) \
					current.prop = mnemonics[i].instr.prop;
			COPY_IF(execZ );
			COPY_IF(execN);
			COPY_IF(input0);
			COPY_IF(input1);
			COPY_IF(command);
			COPY_IF(cmdinfo);
			COPY_IF(flags);
			COPY_IF(output);
			COPY_IF(argument);
			#undef COPY_IF
			// current = mnemonics[i].instr;
			break;
		}
	}
	
	tok = yylex();
	
	apply_modifiers(&tok, &current);
	
	int reqPatch = 0;
	if(tok != TOK_NEWLINE && tok != TOK_EOF)
	{
		switch(tok)
		{
			case TOK_NUM:
				current.argument = getNum(yytext);
				break;
			case TOK_CHAR:
				if(yytext[1] == '\\') {
					switch(yytext[2]) {
						case 'n': current.argument = '\n'; break;
						case 'r': current.argument = '\r'; break;
						case 't': current.argument = '\t'; break;
						case '\'': current.argument = '\''; break;
						case '\"': current.argument = '\"'; break;
					}
				} else {
					current.argument = (uint32_t)yytext[1];
				}
				break;
			case TOK_VAR:
			{
				// (yytext + 1) removes the leading $
				// check if we already had a variable with this name
				uint32_t target = list_find(&variables, yytext + 1);
				if(listFound)
					current.argument = target;
				else {
					// insert patch here for deferred argument modification
					// store the exact position in the file for the patch
					list_insert(&vpatches, yytext + 1, ftell(output) + 4);
					reqPatch = 1;
				}
				break;
			}
			case TOK_REFERENCE:
			{
				// (yytext + 1) removes the leading @
				// check if we already had a label with this name
				uint32_t target = list_find(&labels, yytext + 1);
				if(listFound)
					current.argument = addrToInstr(target);
				else {
					// insert patch here for deferred argument modification
					// store the exact position in the file for the patch
					list_insert(&patches, yytext + 1, ftell(output) + 4);
					reqPatch = 1;
				}
				break;
			}
			default: 
				error(fileName, "Expected number, character or label reference, got '%s' instead.", yytext);
		}
		tok = yylex();
		apply_modifiers(&tok, &current);
	}
	if(tok != TOK_NEWLINE && tok != TOK_EOF)
	{
		error(fileName, "Expected end of line, got '%s instead.", yytext);
	}
	
	if((currentSection->section.length & 0xFFFFFFF8) != currentSection->section.length)
	{
		warning(fileName, "Code Misalignment");
	}
	
	// write command:
	currentSection->section.length += fwrite(&current, 1, sizeof(instruction_t), output);
}

void unescape(char *str)
{
	char *wptr = str;
	char *rptr = str;
	
	while(*rptr)
	{
		if(*rptr != '\\') {
			*wptr++ = *rptr++;
			continue;
		}
		rptr++;
		switch(*rptr)
		{
			case '\\': *wptr++ = '\\'; break;
			case '\"': *wptr++ = '\"'; break;
			case '\'': *wptr++ = '\''; break;
			case 't': *wptr++ = '\t'; break;
			case 'v': *wptr++ = '\v'; break;
			case 'n': *wptr++ = '\n'; break;
			case 'r': *wptr++ = '\r'; break;
			default: *wptr++ = *rptr; break;
		}
		rptr++;
	}
	*wptr = 0;
}

void read_variable(const char *fileName)
{
	char name[64];
	int type = 0;
	
	assert_tok(fileName, TOK_MNEMONIC);
	strcpy(name, yytext);
	
	assert_tok(fileName, TOK_MNEMONIC); // check for type
	if(strcmp(yytext, "INT8") == 0) {
		type = 1;
	}
	else if(strcmp(yytext, "INT16") == 0) {
		type = 2;
	}
	else if(strcmp(yytext, "INT32") == 0) {
		type = 3;
	}
	else if(strcmp(yytext, "UINT8") == 0) {
		type = 4;
	}
	else if(strcmp(yytext, "UINT16") == 0) {
		type = 5;
	}
	else if(strcmp(yytext, "UINT32") == 0) {
		type = 6;
	}
	else if(strcmp(yytext, "STRING") == 0) {
		type = 7;
	}
	else if(strcmp(yytext, "ZSTRING") == 0) {
		type = 8;
	}
	else if(strcmp(yytext, "LSTRING") == 0) {
		type = 9;
	}
	
	list_insert(
		&variables, 
		name, 
		currentSection->section.base + currentSection->section.length);
	
	switch(type)
	{
		case 1:
		case 4:
		{
			assert_tok(fileName, TOK_NUM);
			uint8_t value = getNum(yytext);
			fwrite(&value, 1, 1, output);
			currentSection->section.length += 1;
			break;
		}
		case 2:
		case 5:
		{
			assert_tok(fileName, TOK_NUM);
			uint16_t value = getNum(yytext);
			fwrite(&value, 2, 1, output);
			currentSection->section.length += 2;
			break;
		}
		case 3:
		case 6:
		{
			assert_tok(fileName, TOK_NUM);
			uint32_t value = getNum(yytext);
			fwrite(&value, 4, 1, output);
			currentSection->section.length += 4;
			break;
		}
		case 7: // STRING
		{
			assert_tok(fileName, TOK_STR);
			char *value = yytext + 1;
			int len = strlen(value) - 1;
			value[len] = 0;
			
			unescape(value);
			len = strlen(value);
			
			fwrite(value, len, 1, output);
			currentSection->section.length += len;
			break;
		}
		case 8: // ZSTRING
		{
			assert_tok(fileName, TOK_STR);
			char *value = yytext + 1;
			int len = strlen(value) - 1;
			value[len] = 0;
			
			unescape(value);
			len = strlen(value);
			
			fwrite(value, len + 1, 1, output);
			currentSection->section.length += (len + 1);
			break;
		}
		case 9: // LSTRING
		{
			assert_tok(fileName, TOK_STR);
			char *value = yytext + 1;
			uint32_t len = strlen(value) - 1;
			value[len] = 0;
			
			unescape(value);
			len = strlen(value);
			
			fwrite(&len, 4, 1, output);
			fwrite(value, len, 1, output);
			currentSection->section.length += (len + 4);
			break;
		}
		default:
			error(fileName, "Variable type %s is not a valid type name.", yytext);
	}
}

bool process(const char *fileName)
{
	if(fileName == NULL) {
		lastError = "Invalid file name.";
		return false;
	}
	
	FILE *input = fopen(fileName, "r");
	if(input == NULL) {
		lastError = "Could not find file.";
		return false;
	}
	
	yypush_buffer_state(yy_create_buffer( input, YY_BUF_SIZE ));
	
	while(true)
	{
		int tok = yylex();
		if(tok == TOK_EOF)
		{
			break;
		}
		else if(tok == TOK_INCDIR)
		{
			assert_tok(fileName, TOK_STR);
			char *incfileName = yytext + 1;
			incfileName[strlen(incfileName) - 1] = 0;
			
			int lineno = yyget_lineno();
			yyset_lineno(1);
			
			incfileName = makeIncludeRelative(fileName, incfileName);
			bool success = process(incfileName);
			free(incfileName);
			
			yyset_lineno(lineno);
			
			if(!success) {
				error(fileName, "Could not include file: %s", lastError);
				yyterminate();
				return false;
			}
		}
		else if(tok == TOK_ORGDIR)
		{
			assert_tok(fileName, TOK_NUM);
			uint32_t origin = getNum(yytext);
			
			new_section(".org", origin);
		}
		else if(tok == TOK_ALNDIR)
		{
			assert_tok(fileName, TOK_NUM);
			uint32_t alignment = getNum(yytext);
			
			if(alignment == 0)
				warning(fileName, "Zero alignment is ignored.");
			else
			{
				uint32_t len = currentSection->section.length;
				uint32_t aligned = (len + (alignment - 1)) & ~(alignment - 1);
				
				uint32_t padding = aligned - len;
				for(int i = 0; i < padding; i++) {
					int z = 0;
					fwrite(&z, 1, 1, output);
				}
				currentSection->section.length += padding;
			}
		}
		else if(tok == TOK_VARDIR)
		{
			read_variable(fileName);
		}
		else
		{
			read_instruction(fileName, tok);
		}
	}
	
	yypop_buffer_state();
	
	fclose(input);
	
	return true;
}


void apply_modifier(instruction_t *i, const char *mod)
{
	// printf("[%s]\n", mod);
	if(strncmp("ci:", mod, 3) == 0)
	{
		i->cmdinfo = atoi(mod + 3);
		return;
	}

	if(strncmp("cmd:", mod, 4) == 0)
	{
		fprintf(stderr, "Command specification %s not supported yet.\n", mod);
		exit(1);
	}
	
	if(strcmp("f:yes", mod) == 0)
	{
		i->flags = 1;
	}
	else if(strcmp("f:no", mod) == 0)
	{
		i->flags = 0;
	}
	else if(strcmp("r:discard", mod) == 0)
	{
		i->output = VM_OUTPUT_DISCARD;
	}
	else if(strcmp("r:push", mod) == 0)
	{
		i->output = VM_OUTPUT_PUSH;
	}
	else if(strcmp("r:jump", mod) == 0)
	{
		i->output = VM_OUTPUT_JUMP;
	}
	else if(strcmp("r:jumpr", mod) == 0)
	{
		i->output = VM_OUTPUT_JUMPR;
	}
	else if(strcmp("i0:zero", mod) == 0)
	{
		i->input0 = VM_INPUT_ZERO;
	}
	else if(strcmp("i0:pop", mod) == 0)
	{
		i->input0 = VM_INPUT_POP;
	}
	else if(strcmp("i0:peek", mod) == 0)
	{
		i->input0 = VM_INPUT_PEEK;
	}
	else if(strcmp("i0:arg", mod) == 0)
	{
		i->input0 = VM_INPUT_ARG;
	}
	else if(strcmp("i1:zero", mod) == 0)
	{
		i->input1 = VM_INPUT_ZERO;
	}
	else if(strcmp("i1:pop", mod) == 0)
	{
		i->input1 = VM_INPUT_POP;
	}
	else if(strcmp("ex(z)=x", mod) == 0)
	{
		i->execZ = VM_EXEC_X;
	}
	else if(strcmp("ex(z)=0", mod) == 0)
	{
		i->execZ = VM_EXEC_0;
	}
	else if(strcmp("ex(z)=1", mod) == 0)
	{
		i->execZ = VM_EXEC_1;
	}
	else if(strcmp("ex(n)=x", mod) == 0)
	{
		i->execN = VM_EXEC_X;
	}
	else if(strcmp("ex(n)=0", mod) == 0)
	{
		i->execN = VM_EXEC_0;
	}
	else if(strcmp("ex(n)=1", mod) == 0)
	{
		i->execN = VM_EXEC_1;
	}
	else
	{
		fprintf(stderr, "Unknown modifier: [%s]\n", mod);
		exit(1);
	}
}