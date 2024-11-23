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
	RotateAnimations,
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
	RemoteAnimationController(int recieverPin);

	void startRecieving();
	void startAnimationLoop();

	void waitUnlessInterupted(int waitMillieseconds);

private:
	// uint16_t pollTime = 10;
	int lastDecodeTime = 10;
	CommandCode lastDecodeCommand = 0;
	int recieverPin;
	int lightBrightness = 50;
	Color plainStrandColor = Color::White;
	AnimationMode currentMode = AnimationMode::PlainColor;

	void handleInternalCommand(CommandCode command);
	void setStrandColor(Color color);
	void setStrandBrightness(int brightness);
};