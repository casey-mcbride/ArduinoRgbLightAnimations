#pragma once
// Contains a set of animations for animating a string of led lights

#include "ColorUtils.h"
#include "Basic.h"
#include "ColorPalette.h"
#include "ColorPairPalette.h"

typedef unsigned long ulong;
#ifdef DEBUG
	const ulong animationMilliSeconds = (ulong)12 * (ulong)1000;// need to cast, otherwise you get int max, idky
#else
	const ulong animationMilliSeconds = (ulong)90 * (ulong)1000;
#endif

bool shouldContinueAnimation(ulong start)
{
	delay(30);
	ulong currentTime = millis();
	bool shouldContinue = currentTime - start < animationMilliSeconds;
	return shouldContinue;
}

#define WHILE_ANIMATION_LOOP ulong whileAnimationLoopStartTime = millis();\
	while(shouldContinueAnimation(whileAnimationLoopStartTime))

const int BEAM_ANIMATION_DELAY = 20;

// Marches throught the given colors, with bandSize number of colors repeated in a row
void colorMarch(Color* colors, int numColors, const int bandSize)
{
	int cycleOffset = 0;

	WHILE_ANIMATION_LOOP
	{
		for(int i = 0; i < NUM_LEDS; i++)
		{
			Color c = colors[((i + cycleOffset) / bandSize) % numColors];
			setLed(i, c);
		}

		FastLED.show();
		delay(500);
		cycleOffset++;
	}
}


// Animation that has two colors moving together in one direction, with one color then the next with them lerped together
void mixedWaveAnimation(Color color1, Color color2)
{
	int cycleOffset = 0;
	const int WAVE_LENGTH = 30;
	int cyclePosition = 0;

	WHILE_ANIMATION_LOOP
	{
		for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
		{
			int localPosition = (ledIndex + cyclePosition) % WAVE_LENGTH;
			int distanceFromPeak = abs(localPosition - WAVE_LENGTH / 2);
			float lerpValue = (float)distanceFromPeak / (WAVE_LENGTH / 2);

			Color interpolatedColor = colorLerp(lerpValue, color1, color2);
			setLed(ledIndex, interpolatedColor);
		}
		FastLED.show();
		delay(60);
		cyclePosition = (cyclePosition + 1) % WAVE_LENGTH;
	}
}

void randomMixedWaveAnimation()
{
	Color color1, color2;
	getNextRandomColorPair(color1, color2);

	Color pairs[] =
	{
		ROYAL_BLUE,
		WHITE,

		LIME_GREEN,
		WHITE,

		ORCHID,
		ROYAL_BLUE,

		ROYAL_BLUE,
		YELLOW,

		Color::Red,
		LIME_GREEN,

		SKY_BLUE,
		LAVENDAR,
	};
	const int numPairs = sizeof(pairs) / sizeof(pairs[0]) / 2;
	int pairIndex = random(numPairs);

	mixedWaveAnimation(pairs[pairIndex * 2], pairs[pairIndex * 2 + 1]);
}

void greenBlueThrobAnimation()
{
	int cycleOffset = 0;
	const int TRANSITION_TICKS = 100;
	Color color1 = Color::Green;
	Color color2 = Color::Blue;

	WHILE_ANIMATION_LOOP
	{
		for(int cyclePosition = 0; cyclePosition <= TRANSITION_TICKS; cyclePosition++)
		{
			for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
			{
				int distanceFromPeak = TRANSITION_TICKS - cyclePosition;
				float lerpValue = (float)distanceFromPeak / TRANSITION_TICKS;

				Color interpolatedColor = colorLerp(lerpValue, color1, color2);
				setLed(ledIndex, interpolatedColor);
			}
			FastLED.show();
			delay(60);
		}
		Color temp = color1;
		color1 = color2;
		color2 = temp;
	}
}

void colorThrob(ColorPalette& colorGenerator)
{
	int cycleOffset = 0;
	const int TRANSITION_TICKS = 100;
	Color color1 = colorGenerator.getNextRandomColor();
	Color color2 = colorGenerator.getNextRandomColor();

	WHILE_ANIMATION_LOOP
	{
		for(int cyclePosition = 0; cyclePosition <= TRANSITION_TICKS; cyclePosition++)
		{
			for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
			{
				int distanceFromPeak = TRANSITION_TICKS - cyclePosition;
				float lerpValue = (float)distanceFromPeak / TRANSITION_TICKS;

				Color interpolatedColor = colorLerp(lerpValue, color1, color2);
				setLed(ledIndex, interpolatedColor);
			}
			FastLED.show();
			delay(60);
		}
		color2 = color1;
		color1 = colorGenerator.getNextRandomColor();
	}
}

void rainbowColorHillAnimation(ColorPalette& colorGenerator)
{
	const byte trailLength = NUM_LEDS;
	const int BUFFER = 10;
	const int trailDistance = trailLength * 2 + BUFFER;

	WHILE_ANIMATION_LOOP
	{
		Color color = colorGenerator.getNextRandomColor();
		for(int spotPosition = 0 - trailLength; spotPosition < trailDistance; spotPosition++)
		{
			for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
			{
				// Glow things based on distance from the spot
				int distance = abs(spotPosition - ledIndex);
				setLed(ledIndex, colorLerp((float)distance / trailLength, color, Color::Black));

			}

			FastLED.show();
			delay(BEAM_ANIMATION_DELAY);
		}

		// Go reverse direction
		color = colorGenerator.getNextRandomColor();
		for(int spotPosition = 0 - trailLength; spotPosition < trailDistance; spotPosition++)
		{
			for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
			{
				// Glow things based on distance from the spot
				int distance = abs(spotPosition - ledIndex);
				setLed(NUM_LEDS - ledIndex - 1, colorLerp((float)distance / trailLength, color, Color::Black));

			}

			FastLED.show();
			delay(BEAM_ANIMATION_DELAY);
		}
	}
}

void rainbowColorBeamAnimation()
{
	const byte trailLength = NUM_LEDS;
	const byte buffer = 10;

	WHILE_ANIMATION_LOOP
	{
		Color currentColor = getNextRandomColor();
		for(int spotPosition = 0; spotPosition < NUM_LEDS + trailLength + buffer; spotPosition++)
		{
			for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
			{
				if(ledIndex > spotPosition)
				{
					setLed(ledIndex, Color::Black);
				}
				else
				{
					float lerpValue =  clamp((spotPosition - ledIndex) / (float)trailLength, 0.0f, 1.0f);
					setLed(ledIndex, colorLerp(lerpValue, currentColor, Color::Black));
				}
			}

			FastLED.show();
			delay(BEAM_ANIMATION_DELAY);
		}

		// GO back the other way
		currentColor = getNextRandomColor();
		for(int spotPosition = 0; spotPosition < NUM_LEDS + trailLength + buffer; spotPosition++)
		{
			for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
			{
				if(ledIndex > spotPosition)
				{
					setLed(NUM_LEDS - ledIndex - 1, Color::Black);
				}
				else
				{
					float lerpValue =  clamp((spotPosition - ledIndex) / (float)trailLength, 0.0f, 1.0f);
					setLed(NUM_LEDS - ledIndex - 1, colorLerp(lerpValue, currentColor, Color::Black));
				}
			}

			FastLED.show();
			delay(BEAM_ANIMATION_DELAY);
		}
	}
}

void rainbowColorBeamCollisionAnimation()
{
	const byte trailLength = NUM_LEDS;
	const byte buffer = 10;
	const int animationDistance = NUM_LEDS + trailLength + buffer;

	WHILE_ANIMATION_LOOP
	{
		Color leftColor;
		Color rightColor;

		getNextRandomColorPair(leftColor, rightColor);

		for(int spotPosition = 0; spotPosition < animationDistance; spotPosition++)
		{
			int reverseSpotPosition = NUM_LEDS - spotPosition;

			for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
			{
				float lerpValue1;
				float lerpValue2;
				if(ledIndex > spotPosition)
					lerpValue1 = 1.0f;
				else
					lerpValue1 =  clamp((spotPosition - ledIndex) / (float)trailLength, 0.0f, 1.0f) / 2.0f + 0.5f;

				if(ledIndex < reverseSpotPosition)
					lerpValue2 = 1.0f;
				else
					lerpValue2 =  clamp((ledIndex - reverseSpotPosition) / (float)trailLength, 0.0f, 1.0f) / 2.0f + 0.5f;

				Color one = colorLerp(lerpValue1, leftColor, Color::Black);
				Color two = colorLerp(lerpValue2, rightColor, Color::Black);

				setLed(ledIndex, one + two);
			}

			FastLED.show();
			delay(BEAM_ANIMATION_DELAY);
		}
	}
}

void randomBrightSpots(ColorPalette& generator, int fadeTicks)
{
	const int COLOR_CHANCE = 25;
	WHILE_ANIMATION_LOOP
	{
		for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
		{
			Color current = getLed(ledIndex);
			current = dimColor(current, fadeTicks);
			if(isBlack(current))
			{
				if(random(COLOR_CHANCE) == 0)
				{
					current = generator.getAnyRandomColor();
				}
			}
			setLed(ledIndex, current);
		}
		FastLED.show();
		delay(30);
	}
}

// One color goes up, making it solid then another color merges down merging and erasing the covered one, rinse repeat
void lineSwap(ColorPalette& colorGenerator)
{
	const int WINDOW_SIZE = 50;
	const int LINE_SWAP_DELAY = 70;
	Color current = Color::Black;
	for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
	{
		setLed(ledIndex, current);
	}
	FastLED.show();
	bool goingRight = true;

	const int SPOT_RANGE = NUM_LEDS + WINDOW_SIZE;

	WHILE_ANIMATION_LOOP
	{
		Color next = colorGenerator.getNextRandomColor();
		if(goingRight)
		{
			for(int spotPosition = 0; spotPosition < SPOT_RANGE; spotPosition++)
			{
				for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
				{
					float lerpValue;
					if(ledIndex > spotPosition)
						lerpValue = 1.0f;
					else if(ledIndex < spotPosition - WINDOW_SIZE)
						lerpValue = 0.0f;
					else
						lerpValue =  1.0f - (float)(spotPosition - ledIndex) / WINDOW_SIZE;

					Color color = colorLerp(lerpValue, next, current);
					setLed(ledIndex, color);
				}

				FastLED.show();
				delay(LINE_SWAP_DELAY);
			}
		}
		else
		{
			for(int spotPosition = SPOT_RANGE; spotPosition >= 0; spotPosition--)
			{
				for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
				{
					float lerpValue;
					if(ledIndex < spotPosition - WINDOW_SIZE)
						lerpValue = 1.0f;
					else if(ledIndex > spotPosition)
						lerpValue = 0.0f;
					else
						lerpValue =  (float)(spotPosition - ledIndex) / WINDOW_SIZE;

					Color color = colorLerp(lerpValue, next, current);
					setLed(ledIndex, color);
				}

				FastLED.show();
				delay(LINE_SWAP_DELAY);
			}
		}

		goingRight = !goingRight;
		current = next;

		// Hold color for a bit
		delay(500);
	}
}

void rainbowColorThrob()
{
	colorThrob(COMPLEX_RAINBOW_COLOR_GENERATOR);
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
	colorMarch(colors, getStaticArraySize(colors), 2);
}

void greenBlueWavesAnimation()
{
	mixedWaveAnimation(LIME_GREEN, ROYAL_BLUE);
}

void rainbowLineSwap()
{
	lineSwap(COMPLEX_RAINBOW_COLOR_GENERATOR);
}

void candyCornMarch()
{
	Color colors[] = {Color::White, Color::OrangeRed, GOLDEN_YELLOW};
	int numColors = sizeof(colors)/sizeof(colors[0]);

	colorMarch(colors, numColors, 2);
}

void halloweenBlinkAnimation()
{
	Color colors[] = {Color::Purple, Color::Orange, Color::Green};
	ColorPalette palette(colors, getStaticArraySize(colors));
	randomBrightSpots(palette, 10);
}

void halloweenLineSwap()
{
	lineSwap(HALLOWEEN_COLOR_GENERATOR);
}

void halloweenMixedWave()
{
	Color one, two;
	HALLOWEEN_COLOR_PAIR_GENERATOR.getNextRandomColorPair(one, two);
	mixedWaveAnimation(one, two);
}

//region Chimney animations

#define RING_WIDTH 6 // LEDS
#define DOUBLE_RING_WIDTH (6 * 2) // LEDS

// void bouncyRing()
// {
// 	const int RAINBOW_LINE_SWAP_DELAY = 70;
// 	for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
// 	{
// 		setLed(ledIndex, Color::Black);
// 	}
// 	FastLED.show();

// 	bool goingUp = true;

// 	Color ringColor = getNextRandomColor();

// 	int ringIndex = 0;

// 	WHILE_ANIMATION_LOOP
// 	{
// 		while(ringIndex + RING_WIDTH < NUM_LEDS)
// 		{
// 			for

// 			delay(500);
// 		}

// 		delay(500);
// 	}
// }

void setRingColor(int startRingIndex, Color color)
{
	int startIndex = clamp(startRingIndex, 0, NUM_LEDS - 1);

	// If we're more than a ring away, just don't draw it
	if(startRingIndex - startIndex > RING_WIDTH)
		return;

	int lastLedIndex = clamp(startRingIndex + RING_WIDTH, 0, NUM_LEDS - 1);

	for(int ledIndex = startIndex; ledIndex <= lastLedIndex; ledIndex++)
		setLed(ledIndex, color);
}

void setDoubleRingColor(int startRingIndex, Color color)
{
	int startIndex = clamp(startRingIndex, 0, NUM_LEDS - 1);

	// If we're more than a ring away, just don't draw it
	if(startRingIndex - startIndex > DOUBLE_RING_WIDTH)
		return;

	int lastLedIndex = clamp(startRingIndex + DOUBLE_RING_WIDTH, 0, NUM_LEDS - 1);

	for(int ledIndex = startIndex; ledIndex <= lastLedIndex; ledIndex++)
		setLed(ledIndex, color);
}

void candyCaneSpiral()
{
	const int maxOffset = DOUBLE_RING_WIDTH * 2;
	WHILE_ANIMATION_LOOP
	{
		for(int offset = 0; offset < maxOffset; offset++)
		{
			bool isWhite = false;
			for(int ledIndex = -DOUBLE_RING_WIDTH + offset; ledIndex < NUM_LEDS; ledIndex += DOUBLE_RING_WIDTH)
			{
				if(isWhite)
					setDoubleRingColor(ledIndex, DIM_WHITE);
				else
					setDoubleRingColor(ledIndex, Color::Red);

				isWhite = !isWhite;
			}
			FastLED.show();
			delay(400);
		}
	}
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

//endregion Chimeny animations

using AnimationFunction = void(void);
AnimationFunction* ANIMATIONS[] =
{
	&rainbowColorHillAnimation,
	&rainbowColorBeamCollisionAnimation,
	&greenBlueWavesAnimation,
	&rainbowColorBeamAnimation,
	&greenBlueThrobAnimation,
	&linnaeusFavoriteBrightSpotsAnimation,
	&randomMixedWaveAnimation,
	&rainbowLineSwap,
};
int NUM_ANIMATIONS = sizeof(ANIMATIONS)/ sizeof(ANIMATIONS[0]);

AnimationFunction* HALLOWEEN_ANIMATIONS[] =
{
	&halloweenMixedWave,
	&halloweenLineSwap,
	&halloweenBlinkAnimation,
	&halloweenMixedWave,
	&candyCornMarch,
};
int NUM_HALLOWEEN_ANIMATIONS = sizeof(HALLOWEEN_ANIMATIONS)/ sizeof(HALLOWEEN_ANIMATIONS[0]);

AnimationFunction* CHIMNEY_ANIMATIONS[] =
{
	// &waterFill,
	&rainbowColorThrob,
	&candyCaneMixedWave,
	&fireGlow,
	&christmasBrightSpots,
	&rainbowColorHillAnimation,
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

void doSingleStrandAnimationLoop()
{
	for(int animationIndex = 0; animationIndex < NUM_CHIMNEY_ANMIATIONS; animationIndex++)
	{
		debug("Starting animation: ");
		debug(animationIndex);
		CHIMNEY_ANIMATIONS[animationIndex]();
	}
}
