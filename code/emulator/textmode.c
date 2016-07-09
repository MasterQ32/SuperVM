#include "emulator.h"

#include <stdlib.h>
#include <unistd.h>

extern bool debugMode;


void run_text_mode()
{
	while (running)
	{
		//TODO: Insert some kind of text events.
		update_vm();
		if(debugMode) usleep(250000);
	}
}