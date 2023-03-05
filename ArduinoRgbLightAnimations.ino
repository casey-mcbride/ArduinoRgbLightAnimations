#include "FastLED.h"
#include "ColorUtils.h"
#include "Animations.h"
#include "Basic.h"

#define DATA_PIN 6
#define MAX_BRIGHTNESS 100
// typedef void* (int) AnimationFunction;
using AnimationFunction = void(void);
AnimationFunction* ANIMATIONS[] =
{
	&rainbowColorHillAnimation,
	// &movingRainbowAnimation,
	// &rainbowBeamOverwriteAnimation,
	// &greenRedAlternateAnimation,
	&rainbowColorBeamCollisionAnimation,
	&greenBlueWavesAnimation,
	&rainbowColorBeamAnimation,
	&greenBlueThrobAnimation,
	&linnaeusFavoriteBrightSpotsAnimation,
	&randomMixedWaveAnimation,
	&rainbowLineSwap,
};
int NUM_ANMIATIONS = sizeof(ANIMATIONS)/ sizeof(ANIMATIONS[0]);

void setup()
{
	initDebugIO();

	// If nothing in 0, noise produces a semi-random number
	randomSeed(analogRead(0));

	FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
	FastLED.setBrightness(MAX_BRIGHTNESS);
	for(int i = 0; i < NUM_LEDS; i++)
	{
		setLed(i, Color::Black);
	}
	FastLED.setBrightness(MAX_BRIGHTNESS);
	FastLED.show();
}

void loop()
{
	for(int animationIndex = 0; animationIndex < NUM_ANMIATIONS; animationIndex++)
	{
		Serial.print("Starting animation: ");
		Serial.println(animationIndex);
		ANIMATIONS[animationIndex]();
	}
}