
#include "math.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include "auxiliars.h"
#include <stdlib.h>
#include <algorithm>
//#include <Windows.h>
using namespace std;


//Essa classe tem umas funçõezinhas auxiliares.Tem uma cacetada de coisa que eu não uso.
//Na verdade acho que só o split e o removeVectorfromAnother que eu uso.
//PS: O split eu tive que fazer esse método aí por causa que o compilador lá do ruby é antigo e cheio de frescura. Aí não reconhece alguns métodos novos.

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
double calculateLinkDelay(double quality, int pckSize, double rate, int technology)
{
	double numOfRetries = getNumberOfRetries(quality);
	double ret = -1;
	if (technology == t802_11_g)
	{
		ret = ((pckSize / (rate*pow(2, 20)))*(numOfRetries / round(numOfRetries))) * 1000; //*1000 pra ser milisegundos
		ret += getBackoffTime(quality, technology);
	}
	if (technology == t802_15_4)
	{
		ret = ((pckSize / (250 * pow(2, 10)))*(numOfRetries / round(numOfRetries))) * 1000; //*1000 pra ser milisegundos
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
void readConfiguration(int *scenario,int* technology, double* H_TX, double *H_RX, double *BIT_RATE, double *TRANSMITTER_POWER,int *SRD,int *meshEnabled)
{
	//scanf("%d", scenario);
	//scanf("%d", technology);
	//scanf("%lf", H_TX);
	//scanf("%lf", H_RX);
	//scanf("%lf", BIT_RATE);
	//scanf("%lf", TRANSMITTER_POWER);
	//scanf("%d", SRD);
	//scanf("%d", meshEnabled);
	//FILE *file;
	//fopen_s(&file, "C:\\Sites\\first_app\\teste.txt", "r");
	//fscanf(file,"%d");
	//fscanf(file,"%d", scenario);
	//fscanf(file, "%d", technology);
	//fscanf(file, "%lf", H_TX);
	//fscanf(file, "%lf", H_RX);
	//fscanf(file, "%lf", BIT_RATE);
	//fscanf(file, "%lf", TRANSMITTER_POWER);
	//fscanf(file, "%d", SRD);
	//fscanf(file, "%d", meshEnabled);
	//fclose(file);
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
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}
vector<Position*> removeVectorFromAnother(vector<Position*> &v1, vector<Position*> &v2)
{
	//remove v2 do v1
	vector<Position*> ret;
	for (int i = 0; i < v1.size(); i++)
	{
		std::vector<Position*>::iterator it;
		it = find(v2.begin(), v2.end(), v1[i]);
		if (it == v2.end())
		{
			ret.push_back(v1[i]);
		}
	}
	return ret;
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

int binary_search( vector<Position*>& sorted_vec, double key,int latOrlng) 
{
	size_t mid = 0, left = 0;
	size_t right = sorted_vec.size(); // one position passed the right end
	while (left != right) 
	{
		mid = left + (right - left) / 2;
		if (latOrlng == 0)
		{
			if (key > sorted_vec[mid]->latitude)
			{
				left = mid + 1;
			}
			else if (key <= sorted_vec[mid]->latitude)
			{
				right = mid;
			}
		}
		else
		{
			if (key > sorted_vec[mid]->longitude)
			{
				left = mid + 1;
			}
			else if (key <= sorted_vec[mid]->longitude)
			{
				right = mid;
			}
		}
		
	}

	return left;//ou right
}
//bool compareByLongitude(Position* a, Position *b)
//{
//	return a->longitude < b->longitude;
//}
//bool compareByLatitude(Position* a, Position *b)
//{
//	return a->latitude < b->latitude;
//}
/*size_t getTotalSystemMemory()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	return status.ullTotalPhys;
}*/
vector<Position*> getRegionFromVector(vector<Position*> v, Position* reference, double d)
{
	
	int ind1 = binary_search(v,reference->latitude-d,0);
	int ind2 = binary_search(v, reference->latitude + d,0);
	vector<Position*>::const_iterator first = v.begin() + ind1;
	vector<Position*>::const_iterator last = v.begin() + ind2;
	vector<Position*> newVec(first, last);
//	sort(newVec.begin(), newVec.end(), compareByLongitude);
	ind1 = binary_search(newVec, reference->longitude - d,1);
	ind2 = binary_search(newVec, reference->longitude + d,1);
	vector<Position*>::const_iterator first2 = newVec.begin() + ind1;
	vector<Position*>::const_iterator last2 = newVec.begin() + ind2;
	vector<Position*> newVec2(first2, last2);

	return newVec;
}


//double getHataSRDSuccessRate(double distance, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD)
//{
//	return 1 - bit_error_probability(env, technology, bit_rate, transmitter_power,  h_tx,  h_rx,  distance/1000, SRD);
//	
//
//}
//
////BER para diversas tecnologias
////2014-07-14
//
////Calculo da taxa de erro de bits
//double bit_error_probability(int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, double d, bool SRD)
//{
//
//	//N_0 para cada tipo de ambiente
//
//	double N_0, frequency = -1, E_b = -1, gama_b = -1, bit_error = -1;
//	double signal_power = -1, signal_power_W = -1;
//
//	if (env == Urbano)
//		N_0 = 3.1548e-19;
//	if (env == Suburbano)
//		N_0 = 7.0175e-20;
//	if (env == Rural)
//		N_0 = 4.14275e-21;
//
//
//	//Escolha de tecnologia 802.11g
//	if (technology == t802_11_g){
//		frequency = 2400;	//[MHz]
//
//		signal_power = transmitter_power - loss(frequency, h_tx, h_rx, d, env, SRD);
//
//		//signal_power_W = (10.^(signal_power/10))/1000;
//		signal_power_W = (pow(10, (signal_power / 10))) / 1000;
//		E_b = signal_power_W / (bit_rate*(pow(10, 6)));
//
//		gama_b = E_b / N_0;
//
//		//BPSK
//		if ((bit_rate == 6) || (bit_rate == 9))
//			bit_error = 0.5 * erfc(pow(gama_b, 0.5));
//
//		//QPSK
//		else if ((bit_rate == 12) || (bit_rate == 18))
//			bit_error = 0.5 * erfc(pow(gama_b, 0.5));
//
//		//16-QAM 
//		else if ((bit_rate == 24) || (bit_rate == 36))
//			bit_error = 0.5 * erfc(pow((0.4*gama_b), 0.5));
//
//		//64-QAM
//		else if ((bit_rate == 48) || (bit_rate == 54))
//			bit_error = (1.0 / 3.0) * erfc((pow((9 / 63)*gama_b, 0.5)));
//
//	}
//
//	//Escolha de tecnologia 802.11a	
//	if (technology == t802_11_a){
//		frequency = 5400;	//[MHz]
//
//		signal_power = transmitter_power - loss(frequency, h_tx, h_rx, d, env, SRD);
//
//		//signal_power_W = (10.^(signal_power/10))/1000;
//		signal_power_W = (pow(10, (signal_power / 10))) / 1000;
//		E_b = signal_power_W / (bit_rate*(pow(10, 6)));
//
//		gama_b = E_b / N_0;
//
//		//BPSK
//		if ((bit_rate == 6) || (bit_rate == 9))
//			bit_error = 0.5 * erfc(pow(gama_b, 0.5));
//
//		//QPSK
//		else if ((bit_rate == 12) || (bit_rate == 18))
//			bit_error = 0.5 * erfc(pow(gama_b, 0.5));
//
//		//16-QAM 
//		else if ((bit_rate == 24) || (bit_rate == 36))
//			bit_error = 0.5 * erfc(pow((0.4*gama_b), 0.5));
//
//		//64-QAM
//		else if ((bit_rate == 48) || (bit_rate == 54))
//			bit_error = (1.0 / 3.0) * erfc(pow(((9 / 63)*gama_b), 0.5));
//
//	}
//
//	//Escolha de tecnologia 802.15.4 Zigbee
//	if (technology == t802_15_4){
//		//frequency = 868;	//[MHz] - Europa
//		//frequency = 915;	//[MHz] - EUA
//		frequency = 2400;	//[MHz] - Brasileiro 
//
//		signal_power = transmitter_power - loss(frequency, h_tx, h_rx, d, env, SRD);
//
//		//signal_power_W = (10.^(signal_power/10))/1000;
//		signal_power_W = (pow(10, (signal_power / 10))) / 1000;
//		E_b = signal_power_W / (bit_rate*(pow(10, 6)));
//
//		gama_b = E_b / N_0;
//
//		//BPSK
//		if ((frequency == 868) || (frequency == 915))
//			bit_error = 0.5 * erfc(pow(gama_b, 0.5));
//
//		//OQPSK +/-= QPSK
//		else if ((frequency == 2400))
//			bit_error = 0.5 * erfc(pow(gama_b, 0.5));
//	}
//
//	return bit_error;
//}
//double loss(double f, double h_tx, double h_rx, double d, int environment, bool SRD){
//
//
//	double path_loss = -1;
//	//Como d <= 20km
//	double alpha = 1;
//
//
//	
//	double H_m = fmin(h_tx, h_rx);
//	double H_b = fmax(h_tx, h_rx);
//
//
//	double a = (1.1*log10(f) - 0.7)*fmin(10, H_m) - (1.56*log10(f) - 0.8) + fmax(0, 20 * log10(H_m / 10));
//	double b;
//	if (SRD == false)
//		b = fmin(0.20*log10(H_b / 30), 100); // COLOQUEI ESSE 100 AQUI SÓ PRA COMPILAR, TA ERRADO ESSA PARTE, FALAR COM JOÃO
//	if (SRD == true)
//		b = (((1.1*log10(f)) - 0.7)*fmin(10, H_b)) - (((1.56*log10(f)) - 0.8)) + fmax(0, (20 * log10(H_b / 10)));
//	if (d <= 0.04)
//		path_loss = 32.4 + 20 * log10(f) + 10 * log10((pow(d, 2)) + ((((pow((H_b - H_m), 2))) / (pow(10, 6)))));
//	//path_loss = 32.4 + 20*log10(f) + 10*log10((d.^2) + ((((Math.pow((H_b - H_m),2)))/(Math.pow(10,6)))) );
//
//
//	if (d >= 0.1){
//
//		if (environment == Urbano){
//			if ((30 < f) && (f <= 150))
//				path_loss = 69.9 + 26.2*log10(150) - 20 * log10(150 / f) - 13.82*log10(fmax(30, H_b)) + (44.9 - 6.55*log10(fmax(30, H_b)))*(pow(log10(d), alpha)) - a - b;
//			if ((150 <  f) && (f <= 1500))
//				path_loss = 69.6 + 26.2*log10(f) - 13.82*log10(fmax(30, H_b)) + (44.9 - 6.55*log10(fmax(30, H_b)))*(pow(log10(d), alpha)) - a - b;
//			if ((1500 < f) && (f <= 2000))
//				path_loss = 46.3 + 33.9*log10(f) - 13.82*log10(fmax(30, H_b)) + (44.9 - 6.55*log10(fmax(30, H_b)))*(pow(log10(d), alpha)) - a - b;
//			if ((2000 < f) && (f <= 3000))
//				path_loss = 46.3 + 33.9*log10(2000) + 10 * log10(f / 2000) - 13.82*log10(fmax(30, H_b)) + (44.9 - 6.55*log10(fmax(30, H_b))) * (pow(log10(d), alpha)) - a - b;
//		}
//
//		if (environment == Suburbano)
//			path_loss = loss(f, h_tx, h_rx, d, Urbano, SRD) - 2 * (pow((log10(fmin(fmax(150, f), 2000)) / 28), 2)) - 5.4;
//
//		if (environment == Rural)
//			path_loss = loss(f, h_tx, h_rx, d, Urbano, SRD) - (4.78*(pow((log10(fmin(fmax(150, f), 2000))), 2))) + (18.33*log10(fmin(fmax(150, f), 2000))) - 40.94;
//
//	}
//	if ((0.04 < d) && (d < 0.1))
//		path_loss = (loss(f, h_tx, h_rx, 0.04, environment, SRD)) + ((loss(f, h_tx, h_rx, 0.1, environment, SRD)) - (loss(f, h_tx, h_rx, 0.04, environment, SRD)))*((log10(d) - log10(0.04)) / (log10(0.1) - log10(0.04)));
//
//	return path_loss;
//}

