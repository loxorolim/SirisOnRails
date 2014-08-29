#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include "auxiliars.h"
using namespace std;

float rad(float x)
{
	return x * M_PI / 180;
}
float getDistance(Position p1, Position p2)
{
	float R = 6378137; // Earth’s mean radius in meter
	float dLat = rad(p2.latitude - p1.latitude);
	float dLong = rad(p2.longitude - p1.longitude);
	float a = sin(dLat / 2) * sin(dLat / 2) +
		cos(rad(p1.latitude)) * cos(rad(p2.latitude)) *
		sin(dLong / 2) * sin(dLong / 2);
	float c = 2 * atan2(sqrt(a), sqrt(1 - a));
	float d = R * c;
	return d; // returns the distance in meter
}