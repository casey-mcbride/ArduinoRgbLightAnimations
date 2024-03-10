#include "FastLED.h"
#include "ColorUtils.h"
#include "Animations.h"
#include "LedClusterAnimations.h"
#include "Basic.h"

#define LED_DATA_PIN 6
#define MAX_BRIGHTNESS 100
// typedef void* (int) AnimationFunction;

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

LedClusterAnimator LED_CLUSTER_ANIMATORS[] =
{
	LedClusterAnimator(46, 49),
	LedClusterAnimator(42, 45),
	LedClusterAnimator(38, 41),
	LedClusterAnimator(34, 37),
	LedClusterAnimator(30, 33),
};
const int NUM_LED_CLUSTER_ANIMATORS = sizeof(LED_CLUSTER_ANIMATORS)/ sizeof(LED_CLUSTER_ANIMATORS[0]);

void loop()
{
	//for(int animationIndex = 0; animationIndex < NUM_CHIMNEY_ANMIATIONS; animationIndex++)
	//{
	//	CHIMNEY_ANIMATIONS[animationIndex]();
	//}
	doLedClusterAnimations(LED_CLUSTER_ANIMATORS, NUM_LED_CLUSTER_ANIMATORS);
}