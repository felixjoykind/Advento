#pragma once
#include <random>

class Random
{
public:
	inline static float Float()
	{
		return (float)rand() / RAND_MAX;
	}
};