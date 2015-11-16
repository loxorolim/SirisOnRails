#include "auxiliars.h"


double TIME_LIMIT = 3600;
double MEM_LIMIT = MEM_LIM_DEFAULT;
//Essa classe tem umas funçõezinhas auxiliares.
double getNumberOfRetries(double quality)
{
	double compQuality = 1 - quality;
	return (quality*(1 + 2 * compQuality + 3 * pow(compQuality, 2)) + 4 * pow(compQuality, 3));
}
double getBackoffTime(double quality, int technology)
{
	double ret = -1;
	double compQuality = 1 - quality;
	if (technology == t802_11_g)
		ret = ((quality*(15.5 + 47 * compQuality + 110.5 * pow(compQuality, 2)) + 238 * pow(compQuality, 3)))*(9 / 1000);
	if (technology == t802_15_4)
		ret = ((quality*(3.5 + 11 * compQuality + 26.5 * pow(compQuality, 2)) + 42 * pow(compQuality, 3)))*(320 / 1000);

	return ret;
}
double calculateLinkDelay(double quality, double rate, int technology)
{
	double numOfRetries = getNumberOfRetries(quality);
	double ret = -1;
	if (technology == t802_11_g)
	{
		ret = ((PCK_SIZE / (rate*pow(2, 20)))*(numOfRetries / round(numOfRetries))) * 1000; //*1000 pra ser milisegundos
		ret += getBackoffTime(quality, technology);
	}
	if (technology == t802_15_4)
	{
		ret = ((PCK_SIZE / (250 * pow(2, 10)))*(numOfRetries / round(numOfRetries))) * 1000; //*1000 pra ser milisegundos
		ret += getBackoffTime(quality, technology);
	}
	return ret;
}

double memEstimation(double val1, double val2)
{
	double x = min(val1, val2);
	double estimation;
	if (IS_64)
		estimation = 0.0001536814 * pow(x, 2) + 0.0016503684 * x - 0.0914468615;	
	else
		estimation = 0.0001072724* pow(x, 2) + 0.0010449755*x + 0.0923785966;
	
	estimation *= (max(val1,val2)/min(val1,val2));
	return estimation;

}
double getLongOfDistance(double lat, double distance)
{
	return ((180 * distance) / (M_PI*EARTH_RADIUS*cos(lat*(M_PI / 180))));
}
double getLatOfDistance(double distance)
{
	return ((180 * distance) / (M_PI*EARTH_RADIUS));
}
vector<string> &split(const string &s, char delim, vector<string> &elems)
{
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}
vector<string> split(const string &s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}
double rad(double x)
{
	return x * M_PI / 180;
}
double getDistance(Position* p1, Position* p2)
{
	double R = 6378137; // Earth’s mean radius in meter
	double dLat = rad(p2->latitude - p1->latitude);
	double dLong = rad(p2->longitude - p1->longitude);
	double a = sin(dLat / 2) * sin(dLat / 2) +
		cos(rad(p1->latitude)) * cos(rad(p2->latitude)) *
		sin(dLong / 2) * sin(dLong / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double d = R * c;
	return d; // returns the distance in meter
}
