#include <stdint.h>
#include <stddef.h>

#include "exp.h"


#include <stdio.h>
#include <stdlib.h>

#if defined(_MSC_VER)
#include "getopt.h"
#include <limits.h>
#define PATH_MAX 260 // value from windows.h
#else
#include <getopt.h>
#include <linux/limits.h>
#endif

#include <string.h>
#include <ctype.h>

#define DEBUG_VAL(x) // fprintf(stderr, #x " = %d\n", x)

struct inputfile
{
	char name[PATH_MAX];
	expsection_t section;
	struct inputfile *next;
};

typedef struct inputfile inputfile_t;

inputfile_t *files = NULL;
inputfile_t *lastFile = NULL;

void file_append(uint32_t type, char *name, uint32_t base)
{
	inputfile_t *file = malloc(sizeof(inputfile_t));
	
	strcpy(file->name, name);
	file->next = NULL;
	memset(&file->section, 0, sizeof(expsection_t));
	strncpy(file->section.name, name, 31);
	file->section.type = type;
	file->section.base = base;
	
	if(lastFile == NULL) {
		files = file;
	}
	else {
		lastFile->next = file;
	}
	lastFile = file;
}

void clear_files(void)
{
	while(files != NULL)
	{
		void *tmp = files;
		files = files->next;
		free(tmp);
	}
}

int main(int argc, char **argv)
{
	atexit(clear_files);
	
	char const *outfileName = "a.exp";
	opterr = 0;
	int c;
	
	while ((c = getopt(argc, argv, "E:c:d:m:o:")) != -1)
	{
		switch (c)
		{
		case 'E':
			if(lastFile == NULL) {
				fprintf(stderr, "No current file given.\n");
				exit(1);
			}
			lastFile->section.base = atoi(optarg);
			break;
		case 'o':
			outfileName = optarg;
			break;
		case 'c':
			file_append(0, optarg, 0);
			break;
		case 'd':
			file_append(1, optarg, 0);
			break;
		case '?':
			if (optopt == 'o' || optopt == 'c' || optopt == 'd')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			return 1;
		default:
			abort();
		}
	}

	// for (int index = optind; index < argc; index++)
	// printf("Non-option argument %s\n", argv[index]);

	///////////////////////////////////////////////////////////////////////////////

  if(files == NULL)
  {
    fprintf(stderr, "No input files.\n");
    exit(1);
  }
  
  ///////////////////////////////////////////////////////////////////////////////
    
	FILE *f = fopen(outfileName, "wb");
	if (f == NULL)
	{
		fprintf(stderr, "Could not open file %s\n", outfileName);
		return 1;
	}
  
	///////////////////////////////////////////////////////////////////////////////

	int numSections = 0;
	inputfile_t *it;
	for(it = files; it != NULL; it = it->next)
		numSections += 1;

	///////////////////////////////////////////////////////////////////////////////

	expfile_t fileHeader = {
		EXP_MAGIC,
		1, 0, // version
		0, numSections, // n(meta), n(sections)
		0, 0, // pos(meta), pos(sections)
	};

	fwrite(&fileHeader, sizeof(expfile_t), 1, f);
	fileHeader.posSections = ftell(f);

	DEBUG_VAL(fileHeader.posSections);

	for(it = files; it != NULL; it = it->next)
		fwrite(&it->section, sizeof(expsection_t), 1, f);

	for(it = files; it != NULL; it = it->next)
	{
		it->section.start = ftell(f);

		FILE *fc = fopen(it->name, "rb");
		if (fc == NULL)
		{
			fprintf(stderr, "Could not open %s\n", it->name);
			fclose(f);
			return 1;
		}

		char buffer[4096];
		while (!feof(fc))
		{
			size_t len = fread(buffer, 1, 4096, fc);
			if (len > 0)
				fwrite(buffer, 1, len, f);
			it->section.length += (uint32_t)len;
		}
		
		DEBUG_VAL(it->section.length);

		fclose(fc);
	}

	// Now write the header again...
	rewind(f);
	fwrite(&fileHeader, sizeof(expfile_t), 1, f);

	fseek(f, fileHeader.posSections, SEEK_SET);
	for(it = files; it != NULL; it = it->next)
		fwrite(&it->section, sizeof(expsection_t), 1, f);

	///////////////////////////////////////////////////////////////////////////////

	return 0;
}