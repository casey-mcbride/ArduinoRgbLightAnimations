#include "RemoteAnimationController.h"
#include "IRremote.hpp"

#define MAX_BRIGHTNESS 100
#define MIN_BRIGHTNESS 5
#define BRIGHTNESS_CHANGE_INCREMENT 5

RemoteAnimationController::RemoteAnimationController(int recieverPin)
{
	this->recieverPin = recieverPin;
}

void RemoteAnimationController::startRecieving()
{
	IrReceiver.begin(recieverPin, ENABLE_LED_FEEDBACK);

	initFastLeds();
	setStrandColor(plainStrandColor);
	FastLED.setBrightness(lightBrightness);
	FastLED.show();

	debug("Starting to recieve");
}

void RemoteAnimationController::startAnimationLoop()
{
	while(true)
	{
		waitUnlessInterupted(100);
	}
}

void RemoteAnimationController::waitUnlessInterupted(int waitMillieseconds)
{
	if (IrReceiver.decode()) 
	{
		CommandCode command = (CommandCode)IrReceiver.decodedIRData.command;
		lastDecodeCommand = command;
		handleInternalCommand(command);
		delay(waitMillieseconds);  // wait a bit
		IrReceiver.resume();
	}
}

void RemoteAnimationController::handleInternalCommand(CommandCode command)
{
	// Macro for handling a plain color command
	#define PlainColorCommandCase(colorName)\
	case CommandCode::colorName:\
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
	FastLED.show();
}

void RemoteAnimationController::setStrandBrightness(int brightness)
{
	lightBrightness = clamp(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
	FastLED.setBrightness(lightBrightness);
	FastLED.show();
}