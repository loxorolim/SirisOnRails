#include <iostream>

#ifndef _AUXILIARS_H
#define _AUXILIARS_H

#define MARGIN_VALUE 0.9
#define GREEN_VALUE 0.98 
#define YELLOW_VALUE 0.95
#define BLUE_VALUE 0.90

#define GREEN 0
#define YELLOW 1
#define BLUE 2
#define M_PI 3.14159265358979323846

#define AUTOPLAN 0
#define DRAW 1
#define METRIC 2
#define HEATGRID 3
#define TEST 4
#define GET_RANGE 6


#define Urbano  0
#define Suburbano  1
#define Rural  2

#define t802_15_4  0
#define t802_11_a  1
#define t802_11_g  2
#define t802_11_b  3

#define EARTH_RADIUS 6378137

#include<string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;


template <typename T>
   std::string to_string(T value)
   {
     //create an output string stream

     std::ostringstream os ;

     //throw the value into the string stream
     os.precision(20);
     os  << value ;

     //convert the string stream into a string and return
     return os.str() ;
   }
template <typename T>
   std::string to_string(T value, int precision)
   {
     //create an output string stream

     std::ostringstream os ;

     //throw the value into the string stream
     os.precision(precision);
     os  << value ;

     //convert the string stream into a string and return
     return os.str() ;
   }

class Position
{
	public: 
		double latitude;
		double longitude;
		int index;
		double weight;
		Position(double lat, double lng)
		{
			latitude = lat;
			longitude = lng;
		}
		Position(double lat, double lng, double w)
		{
			latitude = lat;
			longitude = lng;
			index = (int)w;
			weight = w;
		}
		
		bool operator==(const int& i) const
		{
			return index == i;
		}

};
class DrawInfo
{
public:
	int dashed;
	Position* a;
	Position* b;
	int color;
	double efficiency;
	double distance;
	DrawInfo(Position* p1, Position* p2, double eff, double d, int da)
	{
		
		if (eff >= BLUE_VALUE)
			color = BLUE;
		if (eff >= YELLOW_VALUE)
			color = YELLOW;
		if (eff >= GREEN_VALUE)
			color = GREEN;

		a = p1;
		b = p2;

		efficiency = eff;
		distance = d;
		dashed = da;
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
		ret += to_string(dashed);
		//ret += "/";
		return ret;
	}

};
class DrawInfo2
{
public:
	int dashed;
	int a,b,hopnumber;
	double efficiency;
	double distance;
	DrawInfo2(int mainIndex, int toConnectIndex, int hn, double eff, double d, int da)
	{
		a = mainIndex;
		b = toConnectIndex;
		hopnumber = hn;
		efficiency = eff;
		distance = d;
		dashed = da;
	};
	string toString()
	{
		string ret;
		ret += to_string(a);
		ret += "/";
		ret += to_string(b);
		ret += "/";
		ret += to_string(hopnumber);
		ret += "/";
		ret += to_string(efficiency);
		ret += "/";
		ret += to_string(distance);
		ret += "/";
		ret += to_string(dashed);
		//ret += "/";
		return ret;
	}

};

//class sComponent
//{
//	public:
//		int index;
//		double distance;
//		double efficiency;
//		int hop;
//		sComponent *meshFather;
//		sComponent(int i, double d, double e, int h, sComponent * mf)
//		{
//			index = i;
//			distance = d;
//			efficiency = e;
//			hop = h;
//			meshFather = mf;
//		}
//};
//void deleteVector(vector<sComponent*> &v);

double rad(double x);
double getDistance(Position * p1, Position * p2);

//Calculo da BER
//2014-07-14



//double getHataSRDSuccessRate(double distance, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD);
//double bit_error_probability(int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, double d, bool SRD);
//double loss(double f, double h_tx, double h_rx, double d, int environment, bool SRD);

void readConfiguration(int *scenario, int* technology, double* H_TX, double *H_RX, double *BIT_RATE, double *TRANSMITTER_POWER, int *SRD, int *meshEnabled);
int binary_search(vector<Position*>& sorted_vec, double key, int i);
vector<Position*> getRegionFromVector(vector<Position*> v, Position* reference, double d);
size_t getTotalSystemMemory();
vector<string> &split(const string &s, char delim, vector<string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
vector<Position*> removeVectorFromAnother(vector<Position*> &v1, vector<Position*> &v2);
double getLongOfDistance(double lat, double distance);
double getLatOfDistance(double distance);

//bool compareByLatitude(Position* a, Position *b);

#endif

