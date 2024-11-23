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

bool shouldContinueAnimation(ulong start);

#define WHILE_ANIMATION_LOOP ulong whileAnimationLoopStartTime = millis();\
	while(shouldContinueAnimation(whileAnimationLoopStartTime))

const int BEAM_ANIMATION_DELAY = 20;

// Marches throught the given colors, with bandSize number of colors repeated in a row
void colorMarch(ColorPalette& palette, const int bandSize);

// Animation that has two colors moving together in one direction, with one color then the next with them lerped together
void mixedWaveAnimation(Color color1, Color color2);

void colorThrob(ColorPalette& colorGenerator);

// One color beam comes from one direction and fades out, then a different color beam comes from the other, repeat
// The color starts dim, gets bright, gets dim again
void colorHillAnimation(ColorPalette& colorGenerator);

// One color beam comes from one direction and fades out, then a different color beam comes from the other, repeat
// The color starts bright, then gets dim near the tailend
void colorBeamAnimation(ColorPalette& palette);

// Two colors come from both sides and pass through eachother
void colorBeamCollisionAnimation(ColorPairPalette& colorPalette);

// Randomly dim bulbs light up to a color, then fade back to black
void randomBrightSpots(ColorPalette& generator, int fadeTicks);

// One color goes up, making it solid then another color merges down merging and erasing the covered one, rinse repeat
void lineSwap(ColorPalette& colorGenerator);

void firelightAnimation();