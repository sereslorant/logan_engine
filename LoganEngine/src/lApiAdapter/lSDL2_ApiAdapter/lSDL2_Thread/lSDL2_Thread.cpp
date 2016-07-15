
#include "lSDL2_Thread.h"

int lSDL2_ThreadFunction(void *user_data)
{
	lSDL2_Thread *Thread = (lSDL2_Thread *)user_data;

	return Thread->Run();
}
