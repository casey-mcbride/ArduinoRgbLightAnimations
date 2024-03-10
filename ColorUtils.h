#pragma once
#include "Basic.h"

#define NUM_LEDS 50

typedef CRGB Color;

Color leds[NUM_LEDS];

const Color SIMPLE_RAINBOW[] =
{
	Color::Red,
	Color(BYTE_MAX, BYTE_MAX / 3, 0),//Color::Orange
	Color(BYTE_MAX, BYTE_MAX, 0), // Color::Yellow
	Color::Green,
	Color::Teal,
	Color::Blue,
	Color::Purple,
};
const int SIMPLE_RAINBOW_COLOR_COUNT = sizeof(SIMPLE_RAINBOW) / sizeof(SIMPLE_RAINBOW[0]);


const Color DIM_WHITE = Color(80, 80, 80);
const Color WHITE = Color(120, 120, 120);
const Color ORCHID = Color(255, 0, 100);
const Color HOT_PINK = Color::HotPink;

const Color ORANGE = Color::OrangeRed;
const Color GOLDEN_YELLOW = Color::Orange;
const Color PALE_YELLOW = Color::Goldenrod;

const Color YELLOW = Color::Yellow;
const Color PALE_GREEN_YELLOW = Color::YellowGreen;

const Color PEA_GREEN = Color(100, 200, 0);
const Color LIME_GREEN = Color::Green;

const Color SKY_BLUE = Color::SeaGreen;
const Color PALE_BLUE = Color::RoyalBlue;
const Color ROYAL_BLUE = Color::Blue;
const Color PURPLE_BLUE = Color(50, 0, 255);

const Color LAVENDAR = Color(100, 50, 255);
const Color PURPLE = Color::Purple;
const Color PALE_PURPLE = Color(50, 0, 200);

const Color COMPLEX_RAINBOW[] =
{
	// Reds
	ORCHID,
	// Color::Red,
	HOT_PINK,

	// Oranges
	ORANGE,
	GOLDEN_YELLOW,
	PALE_YELLOW,

	// Yellows
	YELLOW,
	PALE_GREEN_YELLOW,

	// Greens
	PEA_GREEN,
	LIME_GREEN,

	SKY_BLUE,
	PALE_BLUE,
	ROYAL_BLUE,
	PURPLE_BLUE,

	LAVENDAR,
	PURPLE,
	PALE_PURPLE,
};
const int COMPLEX_RAINBOW_COLOR_COUNT = sizeof(COMPLEX_RAINBOW) / sizeof(COMPLEX_RAINBOW[0]);

const Color RAINBOW_PAIRS[] =
{
	// ORCHID,
	// ORANGE,

	ORCHID,
	ROYAL_BLUE,

	ORCHID,
	LAVENDAR,

	ORCHID,
	PURPLE_BLUE,

	ORCHID,
	PURPLE,

	HOT_PINK,
	ROYAL_BLUE,

	// HOT_PINK,
	// PURPLE_BLUE,

	HOT_PINK,
	PURPLE,

	// HOT_PINK,
	// PALE_PURPLE,

	// ORANGE,
	// YELLOW,

	ORANGE,
	PURPLE,

	// YELLOW,
	// SKY_BLUE,

	YELLOW,
	LIME_GREEN,

	// PALE_YELLOW,
	// SKY_BLUE,

	PALE_GREEN_YELLOW,
	SKY_BLUE,

	PEA_GREEN,
	SKY_BLUE,

	PEA_GREEN,
	ROYAL_BLUE,

	LIME_GREEN,
	SKY_BLUE,

	LIME_GREEN,
	ROYAL_BLUE,

	LIME_GREEN,
	PALE_BLUE,

	SKY_BLUE,
	LAVENDAR,

	SKY_BLUE,
	PALE_PURPLE,

	ROYAL_BLUE,
	PURPLE,

	// ROYAL_BLUE,
	// PURPLE_BLUE,
};
const int NUM_RAINBOW_PAIRS = sizeof(RAINBOW_PAIRS) / sizeof(RAINBOW_PAIRS[0]) / 2;

inline void setLed( int index, Color color)
{
	assert(index >= 0 && index < NUM_LEDS, "LED index out of bounds");
	index = clamp(index, 0, NUM_LEDS - 1);
	leds[index] = color;
}

inline Color getLed(int index)
{
	assert(index >= 0 && index < NUM_LEDS, "LED index out of bounds");
	index = clamp(index, 0, NUM_LEDS - 1);
	return leds[index];
}

Color dimColor(Color color, byte dimAmount)
{
	byte r = color.r > dimAmount ? color.r - dimAmount : 0;
	byte g = color.g > dimAmount ? color.g - dimAmount : 0;
	byte b = color.b > dimAmount ? color.b - dimAmount : 0;

	return Color(r, g, b);
}

bool isBlack(Color color)
{
	return color.r == 0 && color.g == 0 && color.b == 0 ;
}

Color colorLerp(float lerpValue, Color color1, Color color2)
{
	lerpValue = clamp(lerpValue, 0.0f, 1.0f);

	byte r = lerp(lerpValue, color1.r, color2.r);
	byte g = lerp(lerpValue, color1.g, color2.g);
	byte b = lerp(lerpValue, color1.b, color2.b);

	return Color(clamp(r, BYTE_MIN, BYTE_MAX), clamp(g, BYTE_MIN, BYTE_MAX), clamp(b, BYTE_MIN, BYTE_MAX));
}

Color colorAverage(Color color1, Color color2, Color color3, float weight1, float weight2, float weight3)
{
	float weightSum = weight1 + weight2 + weight3;
	float r = (color1.r * weight1 + color2.r * weight2 + color3.r * weight3) / weightSum;
	float g = (color1.g * weight1 + color2.g * weight2 + color3.g * weight3) / weightSum;
	float b = (color1.b * weight1 + color2.b * weight2 + color3.b * weight3) / weightSum;

	Serial.println(r);

	return Color(clampToByte((int)r), clampToByte((int)g), clampToByte((int)b));
}

class NextColorGenerator
{
	int randomColorIndex = 0;

public:
	NextColorGenerator()
	{
		randomColorIndex = random(COMPLEX_RAINBOW_COLOR_COUNT);
	}

	Color getNextRandomColor()
	{
		randomColorIndex = getNextRandomExclusive(randomColorIndex, COMPLEX_RAINBOW_COLOR_COUNT);

		return COMPLEX_RAINBOW[randomColorIndex];
	}
};

static NextColorGenerator colorGenerator;
Color getNextRandomColor()
{
	colorGenerator.getNextRandomColor();
}

int randomColorPairIndex = 0;
void getNextRandomColorPair(Color& color1, Color& color2)
{
	randomColorPairIndex = getNextRandomExclusive(randomColorPairIndex, NUM_RAINBOW_PAIRS);

	// Also randomize which one is 1, and which is 2
	int offsetIndex = randomColorPairIndex * 2;
	if(random(2) == 0)
	{
		color1 = RAINBOW_PAIRS[offsetIndex];
		color2 = RAINBOW_PAIRS[offsetIndex + 1];
	}
	else
	{
		color1 = RAINBOW_PAIRS[offsetIndex + 1];
		color2 = RAINBOW_PAIRS[offsetIndex];
	}
}
