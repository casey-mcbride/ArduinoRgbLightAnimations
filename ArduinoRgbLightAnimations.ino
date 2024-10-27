#include "FastLED.h"
#include "ColorUtils.h"
#include "BasicAnimations.h"
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

void rainbowColorBeamCollisionAnimation()
{
	colorBeamAnimation(COMPLEX_RAINBOW_COLOR_PALETTE);
}

void rainbowColorThrob()
{
	colorThrob(COMPLEX_RAINBOW_COLOR_PALETTE);
}

void randomMixedRainbowWaveAnimation()
{
	Color color1, color2;
	COMPLEX_RAINBOW_COLOR_PAIR_PALETTE.getNextRandomColorPair(color1, color2);

	mixedWaveAnimation(color1, color2);
}

// Meant to resemble the glow of a fire
void fireGlow()
{
	Color emberColors[] = {Color(50, 0, 0), Color(40, 0, 0), Color(30, 0, 0), Color(20, 0, 0), Color(10, 0, 0), Color(20, 20, 0), Color(20, 10, 0), Color(30, 10, 0)};
	ColorPalette palette(emberColors, getStaticArraySize(emberColors));
	randomBrightSpots(palette, 1);
}

void linnaeusFavoriteBrightSpotsAnimation()
{
	Color linnaeusColors[] = {Color::Green, Color::Blue};
	ColorPalette palette(linnaeusColors, getStaticArraySize(linnaeusColors));
	randomBrightSpots(palette, 10);
}

void greenBlueMarchAnimation()
{
	Color colors[] = {Color::Green, Color::Blue};
	ColorPalette palette(colors, getStaticArraySize(colors));

	colorMarch(palette, 2);
}

void greenBlueThrobAnimation()
{
	Color colors[] = {Color::Green, Color::Blue};
	ColorPalette palette = ColorPalette(colors, getStaticArraySize(colors));

	colorThrob(palette);
}

void greenBlueWavesAnimation()
{
	mixedWaveAnimation(LIME_GREEN, ROYAL_BLUE);
}

void rainbowLineSwap()
{
	lineSwap(COMPLEX_RAINBOW_COLOR_PALETTE);
}

void rainbowColorHillAnimation()
{
	colorHillAnimation(COMPLEX_RAINBOW_COLOR_PALETTE);
}

void rainbowColorBeamAnimation()
{
	colorBeamAnimation(COMPLEX_RAINBOW_COLOR_PALETTE);
}

void candyCornMarch()
{
	Color colors[] = {Color::White, Color::OrangeRed, GOLDEN_YELLOW};
	ColorPalette palette(colors, getStaticArraySize(colors));

	colorMarch(palette, 2);
}

void halloweenBlinkAnimation()
{
	Color colors[] = {Color::Purple, Color::Orange, Color::Green};
	ColorPalette palette(colors, getStaticArraySize(colors));
	randomBrightSpots(palette, 10);
}

void halloweenLineSwap()
{
	lineSwap(HALLOWEEN_COLOR_PALETTE);
}

void hallloweenBeamCollision()
{
	colorBeamCollisionAnimation(HALLOWEEN_COLOR_PAIR_PALETTE);
}

void hallloweenRedBeamCollision()
{
	Color colors[] = {Color::Red, Color::Red};
	ColorPairPalette palette(colors, getStaticArraySize(colors) / 2);
	colorBeamCollisionAnimation(palette);
}

void halloweenMixedWave()
{
	Color one, two;
	HALLOWEEN_COLOR_PAIR_PALETTE.getNextRandomColorPair(one, two);
	mixedWaveAnimation(one, two);
}

void candyCaneMixedWave()
{
	mixedWaveAnimation(Color::Red, DIM_WHITE);
}

void christmasBrightSpots()
{
	Color christmasColors[] = {Color::Green, Color::Red, DIM_WHITE};
	ColorPalette palette(christmasColors, getStaticArraySize(christmasColors));
	randomBrightSpots(palette, 10);
}

using AnimationFunction = void(void);
AnimationFunction* ANIMATIONS[] =
{
	&rainbowColorHillAnimation,
	&rainbowColorBeamCollisionAnimation,
	&greenBlueWavesAnimation,
	&rainbowColorBeamAnimation,
	&greenBlueThrobAnimation,
	&linnaeusFavoriteBrightSpotsAnimation,
	&randomMixedRainbowWaveAnimation,
	&rainbowLineSwap,
};
int NUM_ANIMATIONS = getStaticArraySize(ANIMATIONS);

AnimationFunction* HALLOWEEN_ANIMATIONS[] =
{
	&halloweenMixedWave,
	&halloweenLineSwap,
	&hallloweenBeamCollision,
	&halloweenBlinkAnimation,
	&candyCornMarch,
	&hallloweenRedBeamCollision,
};
int NUM_HALLOWEEN_ANIMATIONS = getStaticArraySize(HALLOWEEN_ANIMATIONS);

void loop()
{
	for(int animationIndex = 0; animationIndex < NUM_HALLOWEEN_ANIMATIONS; animationIndex++)
	{
		HALLOWEEN_ANIMATIONS[animationIndex]();

		toggleLedDirection();
	}

	if(NUM_HALLOWEEN_ANIMATIONS % 2 == 0)
		toggleLedDirection();
}