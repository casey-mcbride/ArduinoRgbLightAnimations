#pragma once

// #define DEBUG

const byte BYTE_MAX = 255;
const byte BYTE_MIN = 0;

inline void initDebugIO()
{
	#ifdef DEBUG
	// Start io
	Serial.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);
	#endif
}


#ifdef DEBUG
	#define assert(flag, message)\
		if(!(flag)) \
		{\
			delay(1000);\
			Serial.println((message));\
		}

	#define debug(message)\
		Serial.println((message));

	#define debugValue(header, value)\
		Serial.print(header);\
		Serial.print(": ");\
		Serial.println(value);

	#define debugValueIf(flag, header, value)\
		if(flag)\
		{\
			debugValue(header, value);\
		}\

	#define debugBlink(on)\
		Serial.print("Blink: ");\
		Serial.println(on ? "On" : "Off");\
		if(on)\
			digitalWrite(LED_BUILTIN, HIGH);\
		else\
			digitalWrite(LED_BUILTIN, LOW);\
		delay(1000);\
		digitalWrite(LED_BUILTIN, LOW);\
		delay(1000);\


#else
	#define assert(flag, message)
	#define debug(message)
	#define debugValue(header, value)
	#define debugValueIf(flag, header, value)
	#define debugBlink(on)

#endif

template<typename numeric>
numeric clamp(numeric value,numeric minValue, numeric maxValue)
{
	value = max(min(value, maxValue), minValue);
		return value;
}

template<typename numeric>
byte clampToByte(numeric v)
{
	if(v < 0)
		return BYTE_MIN;
	else if(v > BYTE_MAX)
		return BYTE_MAX;
	else
		return (byte)v;
}

template<typename numeric>
numeric lerp(float t, numeric start, numeric stop)
{
	return start * (1 - t) + stop * t;
}

template<typename numeric>
numeric min3(numeric a, numeric b, numeric c)
{
    return min(min(a, b), c);
}

int getNextRandomExclusive(int start, int exclusiveMax)
{
	// Choose a next random number, but prevent it from being the same one
	start = start + random(exclusiveMax - 1);
	start = (start + 1) % exclusiveMax;

	return start;
}

int random(int inclusiveMin, int exclusiveMax)
{
	return random(exclusiveMax - inclusiveMin) + inclusiveMin;
}

bool randomBool()
{
	return random(2);
}

// template<typename TypeOne, typename TypeTwo>
// struct Pair
// {
// 	public:
// 	TypeOne One;
// 	TypeTwo Two;

// 	Pair(TypeOne one, TypeTwo two)
// 	{
// 		One = one;
// 		Two = two;
// 	}
// };