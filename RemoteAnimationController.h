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

enum AnimationMode
{
	PlainColor,
	HoldAnimation,
	RandomAnimation,
};

enum CommandCode
{
	IncreaseBrightness = 69,
	DecreaseBrightness = 70,
	Power = 70,

	Red = 68,
	Yellow = 64,
	Purple = 67,

	Green = 7,
	Teal = 21,
	Orange = 9,

	Blue = 22,
	Pink = 25,
	White = 13,

	QuarterHour = 12,
	HalfHour = 24,
	WholeHour = 94,

	Misc1 = 8,
	Misc2 = 28,
};

// A class for controlling animations that recieves input from a infrared remote control
class RemoteAnimationController
{
public:
	static void startRecieving(int receiverPin);
	static void startAnimationLoop();
	static void testIRReceiverLoop();

	// Delays the animation, while also waiting for interupts. Returns true if the current animation should be cancelled and returned from
	static bool delayUnlessInterrupted(int delayMillieseconds);
	static bool shouldCurrentAnimationContinue();

private:

	RemoteAnimationController() = delete;

	static void handleInternalCommand(CommandCode command);
	static void chooseNextRandomAnimation();
	static void setStrandColor(Color color);
	static void setStrandBrightness(int brightness);
};