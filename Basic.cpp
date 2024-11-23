#include "Basic.h"

void initDebugIO()
{
	#ifdef DEBUG
	// Start io
	Serial.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);
	debug("IO initialized");
	#endif
}

int getNextRandomExclusive(int start, int exclusiveMax)
{
	// Choose a next random number, but prevent it from being the same one
	start = start + random(exclusiveMax - 1);
	start = (start + 1) % exclusiveMax;

	return start;
}

int random(int inclusiveMin, int exclusiveMax)
{
	return random(exclusiveMax - inclusiveMin) + inclusiveMin;
}

bool randomBool()
{
	return random(2);
}
