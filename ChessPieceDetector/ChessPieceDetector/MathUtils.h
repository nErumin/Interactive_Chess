#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <cmath>

constexpr double PI = 3.141592;

constexpr double degreeToRadian(double degree)
{
	return degree * (PI / 180.0);
}

#endif // MATHUTILS_H
