// #include "LedClusterAnimations.h"
#include "RemoteAnimationController.h"
#include "Configuration.h"
#include "Basic.h"
#include "ColorUtils.h"
// #include "BasicAnimations.h"
// #include "ColorUtils.h"

// typedef void* (int) AnimationFunction;
RemoteAnimationController controller(2);

// #define LED_DATA_PIN 6
#define MAX_BRIGHTNESS 100
// typedef void* (int) AnimationFunction;

void setup()
{
	initDebugIO();

	// If nothing in 0, noise produces a semi-random number
	randomSeed(analogRead(0));
	controller.startRecieving();
}

void loop()
{
	controller.startAnimationLoop();
}