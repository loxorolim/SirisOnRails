#ifndef _HATA_SRD_H
#define _HATA_SRD_H


#include "auxiliars.h"
#include "math.h"
#include "global.h"
#include <vector>
#include <stdio.h>

#define bpsk  0
#define qpsk  1
#define t16_qam  2
#define t64_qam  3
#define oqpsk 4 
#define dbpsk 5
#define dqpsk 6
#define cck 7

#define hdd 0
#define hard 1
#define sdd 2
#define soft 3

extern double hataTestRange;

double getHataSRDSuccessRate(double distance, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD);
double bit_error_probability(int env, int technology, int  bit_rate, int  transmitter_power, int  h_tx, int  h_rx, int  d, int SRD);
double coded_modulation(int modulation_type, double code_rate, int decode_type, double gamma_b);
double deviation(double d);
double  fading_modulation(int modulation_type, double gamma_b, double diversity_order);
double pskf(double mu, double divOrder);
double loss(double f, double h_tx, double h_rx, double d, int environment, int SRD);
double uncoded_modulation(int modulation_type, double gamma_b);
int nchoosek(int n, int k);
int fac(int n);



void propagationTable();

#endif
