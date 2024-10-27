#pragma once
// Contains a set of animations for animating a string of led lights

#include "ColorUtils.h"
#include "Basic.h"
#include "ColorPalette.h"
#include "ColorPairPalette.h"
#include "ColorSets.h"

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
void colorMarch(ColorPalette& palette, const int bandSize)
{
	int cycleOffset = 0;

	int numColors = palette.getNumColors();

	WHILE_ANIMATION_LOOP
	{
		for(int i = 0; i < NUM_LEDS; i++)
		{
			Color c = palette.getColor(((i + cycleOffset) / bandSize) % numColors);
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

// One color beam comes from one direction and fades out, then a different color beam comes from the other, repeat
// The color starts dim, gets bright, gets dim again
void colorHillAnimation(ColorPalette& colorGenerator)
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

// One color beam comes from one direction and fades out, then a different color beam comes from the other, repeat
// The color starts bright, then gets dim near the tailend
void colorBeamAnimation(ColorPalette& palette)
{
	const byte trailLength = NUM_LEDS;
	const byte buffer = 10;

	WHILE_ANIMATION_LOOP
	{
		Color currentColor = palette.getNextRandomColor();
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
		currentColor = palette.getNextRandomColor();
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

// Two colors come from both sides and pass through eachother
void colorBeamCollisionAnimation(ColorPairPalette& colorPalette)
{
	const byte trailLength = NUM_LEDS;
	const byte buffer = 10;
	const int animationDistance = NUM_LEDS + trailLength + buffer;

	WHILE_ANIMATION_LOOP
	{
		Color leftColor;
		Color rightColor;

		colorPalette.getNextRandomColorPair(leftColor, rightColor);

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

// Randomly dim bulbs light up to a color, then fade back to black
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