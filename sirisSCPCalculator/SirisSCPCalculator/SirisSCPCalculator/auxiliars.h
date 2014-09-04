#ifndef _AUXILIARS_H
#define _AUXILIARS_H

#define MARGIN_VALUE 0.9
#define M_PI 3.14159265358979323846

#define AUTOPLAN 0
#define PROPAGATION 1
#define METRIC 2

#define Urbano  0
#define Suburbano  1
#define Rural  2

#define t802_15_4  0
#define t802_11_a  1
#define t802_11_g  2

using namespace std;

class Position
{
	public: 
		double latitude;
		double longitude;
		Position(double lat, double lng)
		{
			latitude = lat;
			longitude = lng;
		}
};
class DrawInfo
{
public:
	Position* a;
	Position* b;
	int color;
	double efficiency;
	double distance;
	DrawInfo(Position* p1, Position* p2, int c, double eff, double d)
	{
		a = p1;
		b = p2;
		color = c;
		efficiency = eff;
		distance = d;
	};
	string toString()
	{
		string ret;
		ret += to_string(a->latitude) + "<>";
		ret += to_string(a->longitude);
		ret += "/";
		ret += to_string(b->latitude) + "<>";
		ret += to_string(b->longitude);
		ret += "/";
		ret += to_string(color);
		ret += "/";
		ret += to_string(efficiency);
		ret += "/";
		ret += to_string(distance);
		ret += "/";
		return ret;
	}

};

class sComponent 
{
	public:
		int index;
		double distance;
		double efficiency;
		int hop;
		sComponent *meshFather;
		sComponent(int i, double d, double e, int h, sComponent * mf)
		{
			index = i;
			distance = d;
			efficiency = e;
			hop = h;
			meshFather = mf;
		}
};
void deleteVector(vector<sComponent*> &v);

double rad(double x);
double getDistance(Position * p1, Position * p2);

//Calculo da BER
//2014-07-14



double getHataSRDSuccessRate(double distance, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD);
double bit_error_probability(int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, double d, bool SRD);
double loss(double f, double h_tx, double h_rx, double d, int environment, bool SRD);
double log10(double val);
double erf(double x);
double erfc(double x);

#endif
