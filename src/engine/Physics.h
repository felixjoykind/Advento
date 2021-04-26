#pragma once

class Physics
{
public:
	template<class T>
	static T clamp(T min, T max, T value);
};

template<class T>
inline T Physics::clamp(T min, T max, T value)
{
	if (value > max)
		return max;
	else if (value < min)
		return min;
	else
		return value;
}
