#include "RemoteAnimationController.h"
#include "IRremote.hpp"

#define MAX_BRIGHTNESS 100
#define MIN_BRIGHTNESS 0
#define BRIGHTNESS_CHANGE_INCREMENT 5

/*
command: 69
command: 70
command: 71
command: 68
command: 64
command: 67
command: 7
command: 21
command: 9
command: 22
command: 25
command: 13
command: 12
command: 24
command: 94
command: 8
command: 28
*/
#define INCREASE_BRIGHTNESS_COMMAND_CODE 69
#define DECREASE_BRIGHTNESS_COMMAND_CODE 70


RemoteAnimationController::RemoteAnimationController(int recieverPin)
{
	this->recieverPin = recieverPin;

}

void RemoteAnimationController::startRecieving()
{
	IrReceiver.begin(recieverPin, ENABLE_LED_FEEDBACK);

	initFastLeds();
	for(int i = 0; i < NUM_LEDS; i++)
	{
		setLed(i, Color::White);
	}
	FastLED.setBrightness(lightBrigthness);
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
		uint16_t command = IrReceiver.decodedIRData.command;
		debugValue("command", command);
		lastDecodeCommand = command;
		handleInternalCommand(command);
		delay(waitMillieseconds);  // wait a bit
		IrReceiver.resume();
	}
}

void RemoteAnimationController::handleInternalCommand(uint16_t command)
{
	debug("Hnalding command");
	switch(command)
	{
		case INCREASE_BRIGHTNESS_COMMAND_CODE:
			debug("increasing");
			lightBrigthness = clamp(lightBrigthness + BRIGHTNESS_CHANGE_INCREMENT, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
			debugValue("lightBrightness", lightBrigthness);
			FastLED.setBrightness(lightBrigthness);
			FastLED.show();
			break;

		case DECREASE_BRIGHTNESS_COMMAND_CODE:
			lightBrigthness = clamp(lightBrigthness - BRIGHTNESS_CHANGE_INCREMENT, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
			debugValue("lightBrightness", lightBrigthness);
			FastLED.setBrightness(lightBrigthness);
			FastLED.show();
			break;
		default:
			break;
		
}
