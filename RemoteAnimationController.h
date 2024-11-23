#pragma once

#include "FastLED.h"
#include "ColorUtils.h"
#include "BasicAnimations.h"
// #include "LedClusterAnimations.h"
#include "Basic.h"

enum RemoteAnimationControllerSignal
{
	None,
	SkipToNextAnimation,
};

// A class for controlling animations that recieves input from a infrared remote control
class RemoteAnimationController
{
public:
	RemoteAnimationController(int recieverPin);

	void startRecieving();
	void startAnimationLoop();

	void waitUnlessInterupted(int waitMillieseconds);

private:
	// uint16_t pollTime = 10;
	int lastDecodeTime = 10;
	int lastDecodeCommand = 0;
	int recieverPin;
	int lightBrigthness = 50;

	void handleInternalCommand(uint16_t command);

};