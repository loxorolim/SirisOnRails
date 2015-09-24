#include "FatherMethods.h"
#include "HataSRD.h"

double FatherMethods::getLinkQuality(double distance)
{
	return getHataSRDSuccessRate(distance, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd);
}
double FatherMethods::getLinkQualityBetweenMeters(double distance)
{
	return getHataSRDSuccessRate(distance, scenario, technology, bit_rate, t_power, h_tx, h_tx, srd);
}