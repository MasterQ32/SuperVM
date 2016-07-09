#pragma once

#include "exp.h"

struct sectlist
{
	expsection_t section;
	struct sectlist *next;
};

struct llist
{
	char name[128];
	uint32_t value;
	struct llist *next;
};

typedef struct llist llist_t;

typedef struct sectlist sectlist_t;

extern sectlist_t *currentSection;
extern llist_t *labels;
extern llist_t *variables;
extern llist_t *patches;

extern int listFound ;

void new_section(const char *name, uint32_t base);

void list_insert(llist_t **list, const char *name, uint32_t value);

uint32_t list_find(llist_t **list, const char *name);

void list_free(llist_t **list);



static inline uint32_t addrToInstr(uint32_t addr)
{
	return (addr >> 3);
}