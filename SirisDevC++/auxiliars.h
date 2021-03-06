#ifndef _AUXILIARS_H
#define _AUXILIARS_H
#include <iostream>
#include "math.h"
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

//#include <Windows.h>
using namespace std;
using namespace rapidjson;

#define TIME_LIM_DEFAULT 60*1000
// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define IS_64 1
#define MEM_LIM_DEFAULT 6000
#else
#define IS_64 0
#define MEM_LIM_DEFAULT 2000
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define IS_64 1
#define MEM_LIM_DEFAULT 6000
#else
#define IS_64 0
#define MEM_LIM_DEFAULT 3000
#endif
#endif


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
#define KML 7
#define PROCESS_KML 8

#define Urbano  0
#define Suburbano  1
#define Rural  2

#define t802_15_4  0
#define t802_11_g  1
#define t802_11_a  2
#define t802_11_b  3


#define PCK_SIZE 400*8 //400 BYTES
#define PER_HOP_DELAY 2 //2 MS
#define EARTH_RADIUS 6378137
//#define MEM_LIMIT 99999
//#define TIME_LIMIT 2*60*60*1000
#define MEM_EST_SAFETY 1.1

#define VIVO_ID 0
#define CLARO_ID 1
#define OI_ID 2
#define TIM_ID 3

extern double TIME_LIMIT;
extern double MEM_LIMIT;

static const map<int, string> scenario_map = { { Urbano, "Urbano" },
{ Suburbano, "Suburbano" },
{ Rural, "Rural" } };

static const map<int, string> technology_map = { { t802_15_4, "802.15.4 (ZigBee)" },
{ t802_11_a, "802.11a" },
{ t802_11_g, "802.11g" } };


//to_string em que se delimita uma precis�o de casas decimais
template <typename T> std::string to_string(T value, int precision)
   {
     std::ostringstream os ;
     os.precision(precision);
     os  << value ;
     return os.str() ;
   }

class Position
{
	public: 
		double latitude, longitude, weight;
		int index;
		vector<string> signalInfo;
	
		
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
		Position(double lat, double lng, double w,vector<string> sI)
		{
			latitude = lat;
			longitude = lng;
			index = (int)w;
			weight = w;
			signalInfo = sI;
		}
		
		bool operator==(const Position& i) const
		{
			return this->index == i.index;
		}

};

double getDistance(Position * p1, Position * p2);


vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
//vector<Position*> removeVectorFromAnother(vector<Position*> &v1, vector<Position*> &v2);
double getLongOfDistance(double lat, double distance);
double getLatOfDistance(double distance);
double memEstimation(double val1, double val2);
double getNumberOfRetries(double quality);
double getBackoffTime(double quality, int technology);
double calculateLinkDelay(double quality, double rate, int technology);


//bool compareByLatitude(Position* a, Position *b);

#endif

