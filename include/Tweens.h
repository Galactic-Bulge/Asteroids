#ifndef TWEENS_H
#define TWEENS_H

#include "Singleton.h"

class Tweens: public Singleton<Tweens>
{
public:
	float MapRange(float value, float srcMin, float srcMax, float outMin, float outMax)
	{
		return (value - srcMin) / (srcMax - srcMin) * (outMax - outMin) + outMin;
	}

	float EaseInEaseOut(float t)
	{
		if (t < 0.5)
		{
			return pow(t, 2);
		}
		else
		{
			return pow(1-t,2);
		}
	}

	float EaseIn(float t)
	{
		return pow(t, 2);
	}

	float EaseOut(float t)
	{
		return pow(1 - t, 2);
	}
};
#endif