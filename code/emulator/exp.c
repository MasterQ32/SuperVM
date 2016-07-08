#include "exp.h"
#include "emulator.h"

#include <stdio.h>
#include <stdbool.h>

bool load_exp(const char *fileName)
{
	FILE *f = fopen(fileName, "rb");
	if (f == NULL)
	{
		fprintf(stderr, "Could not open file %s\n", fileName);
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////

	expfile_t fileHeader;

	if (fread(&fileHeader, 1, sizeof(expfile_t), f) != sizeof(expfile_t))
	{
		fprintf(stderr, "File %s does not contain a valid header.\n", fileName);
		return false;
	}
	if (fileHeader.magicNumber != EXP_MAGIC)
	{
		fprintf(stderr, "Invalid magic in %s\n", fileName);
		return false;
	}
	if (fileHeader.majorVersion != 1 && fileHeader.minorVersion == 0)
	{
		fprintf(
			stderr, "Invalid version %s: %d.%d\n",
			fileName,
			fileHeader.majorVersion, fileHeader.minorVersion);
		return false;
	}

	for (uint32_t i = 0; i < fileHeader.numSections; i++)
	{
		expsection_t section;

		fseek(f, fileHeader.posSections + i * sizeof(expsection_t), SEEK_SET);
		fread(&section, 1, sizeof(expsection_t), f);

		fseek(f, section.start, SEEK_SET);

		uint8_t *sectionInRam = (uint8_t*)mainCore.memory + section.base;
		int len = fread(sectionInRam, 1, section.length, f);
		if (len != section.length)
			fprintf(stderr, "Read invalid size.\n");
	}

	return true;
}