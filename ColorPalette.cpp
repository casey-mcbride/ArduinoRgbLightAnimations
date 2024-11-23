#include "ColorPalette.h"

ColorPalette::ColorPalette(Color* colors, int numColors)
{
	this->colors = colors;
	this->numColors = numColors;
	randomColorIndex = random(numColors);
}

int ColorPalette::getNumColors()
{
	return numColors;
}

Color ColorPalette::getColor(int colorIndex) const
{
	assert(colorIndex >= 0 && colorIndex < numColors, "Color index out of bounds");
	return colors[colorIndex];
}

Color ColorPalette::getNextRandomColor()
{
	randomColorIndex = getNextRandomExclusive(randomColorIndex, numColors);

	return colors[randomColorIndex];
}

Color ColorPalette::getRandomColor()
{
	int index = random(0, numColors);

	return colors[index];
}
