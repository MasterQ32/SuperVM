#include "emulator.h"

void run_text_mode()
{
	while (running)
	{
		//TODO: Insert some kind of text events.
		update_vm();
	}
}