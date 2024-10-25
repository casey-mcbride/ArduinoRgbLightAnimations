#pragma once
#include "Basic.h"

class ColorPalette
{
	int randomColorIndex = 0;
	Color* colors = nullptr;
	int numColors = 0;

public:
	ColorPalette(Color* colors, int numColors)
	{
		this->colors = colors;
		this->numColors = numColors;
		randomColorIndex = random(numColors);
	}

	Color getNextRandomColor()
	{
		randomColorIndex = getNextRandomExclusive(randomColorIndex, numColors);

		return colors[randomColorIndex];
	}

	Color getAnyRandomColor()
	{
		int index = random(0, numColors);

		return colors[index];
	}
};
