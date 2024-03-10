#pragma once
// Contains a set of animations for animating a string of led lights

#include "ColorUtils.h"
#include "Basic.h"

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

void greenRedAlternateAnimation()
{
	byte brightness;
	bool brightnessIncreasing;
	int animationOffset = 0;
	WHILE_ANIMATION_LOOP
	{
		for(int brightness = 0; brightness < BYTE_MAX; brightness++)
		{
			for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
			{
				if((ledIndex + animationOffset) % 2 == 0)
					setLed(ledIndex, Color(brightness, 0, 0));
				else
					setLed(ledIndex, Color(0, brightness, 0));
			}

			FastLED.show();
			delay(5);
		}

		for(int brightness = BYTE_MAX; brightness > BYTE_MIN; brightness--)
		{
			for(int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++)
			{
				if((ledIndex + animationOffset) % 2 == 0)
					setLed(ledIndex, Color(brightness, 0, 0));
				else
					setLed(ledIndex, Color(0, brightness, 0));
			}

			FastLED.show();
			delay(5);
		}

		animationOffset++;
	}
}

void greenBlueMarchAnimation()
{
	int cycleOffset = 0;

	WHILE_ANIMATION_LOOP
	{
		for(int i = 0; i < NUM_LEDS; i++)
		{
			if((i + cycleOffset) % 5 == 0)
				setLed(i, Color::Blue);
			else
				setLed(i, Color::Green);
		}

		FastLED.show();
		delay(500);
		cycleOffset++;
	}
}

void greenBlueFadeMarchAnimation()
{
	int cycleOffset = 0;
	const int TRAIL_LENGTH = 4;
	const int FADE_ANIMATION_TICKS = 30;

	WHILE_ANIMATION_LOOP
	{
		for(int lerpCycle = 0; lerpCycle < FADE_ANIMATION_TICKS; lerpCycle++)
		{
			for(int i = 0; i < NUM_LEDS; i++)
			{
				int cyclePosition = (i + cycleOffset) % TRAIL_LENGTH;
				if( cyclePosition < TRAIL_LENGTH - 2)
				{
					setLed(i, Color::Green);
				}
				else
				{
					float lerpValue = (float)lerpCycle / FADE_ANIMATION_TICKS;
					// Flip it around, so one is getting bluer, and the last one is getting grener
					if(cyclePosition == TRAIL_LENGTH - 1)
						lerpValue = 1.0f - lerpValue;

					Color interpolatedColor = colorLerp(lerpValue, Color::Green, Color::Blue);
					setLed(i, interpolatedColor);
				}
			}
			FastLED.show();
			delay(30);
		}

		cycleOffset++;
	}
}

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

void greenBlueWavesAnimation()
{
	mixedWaveAnimation(LIME_GREEN, ROYAL_BLUE);
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

void rainbowColorThrob()
{
	int cycleOffset = 0;
	const int TRANSITION_TICKS = 100;
	Color color1 = getNextRandomColor();
	Color color2 = getNextRandomColor();

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
		color1 = getNextRandomColor();
	}
}

void rainbowColorHillAnimation()
{
	const byte trailLength = NUM_LEDS;
	const int BUFFER = 10;
	const int trailDistance = trailLength * 2 + BUFFER;

	WHILE_ANIMATION_LOOP
	{
		Color color = getNextRandomColor();
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
		color = getNextRandomColor();
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

void randomBrightSpots(Color* colors, int numColors, int fadeTicks)
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
					current = colors[random(numColors)];
				}
			}
			setLed(ledIndex, current);
		}
		FastLED.show();
		delay(30);
	}
}

// // Meant to resemble the glow of a fire
void fireGlow()
{
	Color emberColors[] = {Color(50, 0, 0), Color(40, 0, 0), Color(30, 0, 0), Color(20, 0, 0), Color(10, 0, 0), Color(20, 20, 0), Color(20, 10, 0), Color(30, 10, 0)};
	randomBrightSpots(emberColors, sizeof(emberColors)/sizeof(emberColors[0]), 1);
}

void linnaeusFavoriteBrightSpotsAnimation()
{
	Color linnaeusColors[] = {Color::Green, Color::Blue};
	randomBrightSpots(linnaeusColors, sizeof(linnaeusColors)/sizeof(linnaeusColors[0]), 10);
}

// One color goes up, making it solid then another color merges down merging and erasing the covered one, rinse repeat
void rainbowLineSwap()
{
	const int WINDOW_SIZE = 50;
	const int RAINBOW_LINE_SWAP_DELAY = 70;
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
		Color next = getNextRandomColor();
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
				delay(RAINBOW_LINE_SWAP_DELAY);
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
				delay(RAINBOW_LINE_SWAP_DELAY);
			}
		}

		goingRight = !goingRight;
		current = next;
		next = getNextRandomColor();

		// Hold color for a b
		delay(500);
	}
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

// Meant to resemble the glow of a fire
void christmasBrightSpots()
{
	Color christmasColors[] = {Color::Green, Color::Red, DIM_WHITE};
	randomBrightSpots(christmasColors, sizeof(christmasColors)/sizeof(christmasColors[0]), 10);
}

//endregion Chimeny animations

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

void doSingleStrandAnimationLoop()
{
	for(int animationIndex = 0; animationIndex < NUM_CHIMNEY_ANMIATIONS; animationIndex++)
	{
		debug("Starting animation: ");
		debug(animationIndex);
		CHIMNEY_ANIMATIONS[animationIndex]();
	}
}
