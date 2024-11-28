#include "RemoteAnimationController.h"
#include "AnimationSets.h"
// Current remote uses NEC protocol, defining it forces that protocol
#define DECODE_NEC
#include "IRremote.hpp"

#define MAX_BRIGHTNESS 100
#define MIN_BRIGHTNESS 5
#define COMMAND_INPUT_DELAY 10
#define BRIGHTNESS_CHANGE_INCREMENT 5

#ifdef DEBUG
	static const ulong animationMilliSeconds = (ulong)12 * (ulong)1000;// need to cast, otherwise you get int max, idky
#else
	static const ulong animationMilliSeconds = (ulong)90 * (ulong)1000;
#endif

// uint16_t pollTime = 10;
static ulong lastDecodeTime = 0;
static ulong startOfCurrentAnimation = millis();
static CommandCode lastDecodeCommand = 0;
static int recieverPin;
static int lightBrightness = 75;
static Color plainStrandColor = Color::White;
static AnimationMode currentMode = AnimationMode::RandomAnimation;
static int currentAnimationIndex = 0;

void RemoteAnimationController::startRecieving(int recieverPin)
{
	IrReceiver.begin(recieverPin, ENABLE_LED_FEEDBACK);

	initFastLeds();
	// setStrandColor(plainStrandColor);
	FastLED.setBrightness(lightBrightness);
	FastLED.show();

	debug("Starting to recieve");

	currentMode = AnimationMode::RandomAnimation;
}

void RemoteAnimationController::startAnimationLoop()
{
	if(NUM_ANIMATIONS <= 0)
	{
		debug("No animations, so exiting animation loop");
		return;
	}

	debug("Starting animation loop");

	while(true)
	{
		switch(currentMode)
		{
			case AnimationMode::PlainColor:
				delayUnlessInterrupted(100000);
				debug("Doing plain color animation");
			break;
			case AnimationMode::HoldAnimation:
				debug("Doing hold animation");
				startOfCurrentAnimation = millis();
				ANIMATIONS[currentAnimationIndex]();
			break;

			case AnimationMode::RandomAnimation:
				debug("Doing RandomAnimation ");
				startOfCurrentAnimation = millis();
				ANIMATIONS[currentAnimationIndex]();
				// If we're still in the rotation mode, increment
				if(currentMode == AnimationMode::RandomAnimation)
					chooseNextRandomAnimation();
			break;
		}
	}
}

void RemoteAnimationController::chooseNextRandomAnimation()
{
	currentAnimationIndex = getNextRandomExclusive(currentAnimationIndex, NUM_ANIMATIONS);
	toggleLedDirection();
	debugValue("Newly picked animation index", currentAnimationIndex);
}

bool RemoteAnimationController::delayUnlessInterrupted(int delayMillieseconds)
{
	ulong startTime = millis();
	while(millis() - startTime < delayMillieseconds)
	{
		if (IrReceiver.decode()) 
		{
			debugValue("Delaying for: ", delayMillieseconds);
			CommandCode command = (CommandCode)IrReceiver.decodedIRData.command;
			IrReceiver.resume();

			if(command != 0)
			{
				if(command != lastDecodeCommand || lastDecodeTime + COMMAND_INPUT_DELAY < millis())
				{
					lastDecodeTime = millis();

					lastDecodeCommand = command;
					AnimationMode startingMode = currentMode;
					handleInternalCommand(command);

					// If we changed modes, we need to interrupt
					if(startingMode != currentMode)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

void RemoteAnimationController::testIRReceiverLoop()
{
	while(true)
	{
		if (IrReceiver.decode()) 
		{
			// IrReceiver.printIRResultShort(&Serial);

			int command = IrReceiver.decodedIRData.command;
			debugValue("Command", command);
			delay(10);
			IrReceiver.resume();
		}
	}
}

bool RemoteAnimationController::shouldCurrentAnimationContinue()
{
	return currentMode == AnimationMode::HoldAnimation || millis() - startOfCurrentAnimation < animationMilliSeconds;
}

void RemoteAnimationController::handleInternalCommand(CommandCode command)
{
	// Macro for handling a plain color command
	#define PlainColorCommandCase(colorName)\
	case CommandCode::colorName:\
		debug(#colorName);\
		setStrandColor(Color::colorName);\
		break;

	switch(command)
	{
		case CommandCode::IncreaseBrightness:
			setStrandBrightness(lightBrightness + BRIGHTNESS_CHANGE_INCREMENT);
			break;

		case CommandCode::DecreaseBrightness:
			setStrandBrightness(lightBrightness - BRIGHTNESS_CHANGE_INCREMENT);
			break;
		case CommandCode::Misc1:
			currentMode = AnimationMode::HoldAnimation;
			break;

		case CommandCode::Misc2:
			currentMode = AnimationMode::RandomAnimation;
			break;
		PlainColorCommandCase(Red);
		PlainColorCommandCase(Yellow);
		PlainColorCommandCase(Purple);
		PlainColorCommandCase(Green);
		PlainColorCommandCase(Teal);
		PlainColorCommandCase(Orange);
		PlainColorCommandCase(Blue);
		PlainColorCommandCase(Pink);
		PlainColorCommandCase(White);

		default:
			break;
	}
}

void RemoteAnimationController::setStrandColor(Color color)
{
	plainStrandColor = color;
	for(int i = 0; i < NUM_LEDS; i++)
	{
		setLed(i, plainStrandColor);
	}
	currentMode = AnimationMode::PlainColor;
	FastLED.show();
}

void RemoteAnimationController::setStrandBrightness(int brightness)
{
	lightBrightness = clamp(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
	FastLED.setBrightness(lightBrightness);
	FastLED.show();
}