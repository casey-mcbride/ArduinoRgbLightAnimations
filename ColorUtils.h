#pragma once
#include "Configuration.h"
#include "Basic.h"


void initFastLeds();

void setLed( int index, Color color);

Color getLed(int index);

void toggleLedDirection();

Color dimColor(Color color, byte dimAmount);

bool isBlack(Color color);

Color colorLerp(float lerpValue, Color color1, Color color2);

Color colorAverage(Color color1, Color color2, Color color3, float weight1, float weight2, float weight3);