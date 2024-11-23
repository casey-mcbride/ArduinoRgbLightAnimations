#pragma once
#include "Basic.h"

class ColorPairPalette
{
	int randomColorPairIndex = 0;
	Color* colorPairs = nullptr;
	int numColorPairs = 0;

public:
	ColorPairPalette(Color* colorPairs, int numColorPairs);

	void getNextRandomColorPair(Color& color1, Color& color2);
};
