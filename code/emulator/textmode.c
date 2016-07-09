#include "emulator.h"

#include <unistd.h>

extern bool debugMode;

void run_text_mode()
{
	while (running)
	{
		//TODO: Insert some kind of text events.
		update_vm();
		if(debugMode) sleep(1);
	}
}