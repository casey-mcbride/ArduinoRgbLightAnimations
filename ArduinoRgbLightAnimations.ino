#include "FastLED.h"
#include "ColorUtils.h"
#include "Animations.h"
#include "Basic.h"

#define LED_DATA_PIN 6
#define MAX_BRIGHTNESS 100
// typedef void* (int) AnimationFunction;
using AnimationFunction = void(void);
AnimationFunction* ANIMATIONS[] =
{
	&rainbowColorHillAnimation,
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

AnimationFunction* CHIMNEY_ANIMATIONS[] =
{
	// &waterFill,
	&rainbowColorThrob,
	&candyCaneMixedWave,
	&fireGlow,
	&christmasBrightSpots,
	&rainbowColorHillAnimation,
	// &rainbowBeamOverwriteAnimation,
	// &greenRedAlternateAnimation,
	&rainbowColorBeamCollisionAnimation,
	&rainbowLineSwap,
	&candyCaneSpiral,
	&greenBlueWavesAnimation,
	&rainbowColorBeamAnimation,
	&greenBlueThrobAnimation,
	&linnaeusFavoriteBrightSpotsAnimation,
	&randomMixedWaveAnimation,
	&rainbowLineSwap,
};
int NUM_CHIMNEY_ANMIATIONS = sizeof(CHIMNEY_ANIMATIONS)/ sizeof(CHIMNEY_ANIMATIONS[0]);

void setup()
{
	initDebugIO();

	// If nothing in 0, noise produces a semi-random number
	randomSeed(analogRead(0));

	FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
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
	for(int animationIndex = 0; animationIndex < NUM_CHIMNEY_ANMIATIONS; animationIndex++)
	{
		Serial.print("Starting animation: ");
		Serial.println(animationIndex);
		CHIMNEY_ANIMATIONS[animationIndex]();
	}
}