#include "stdafx.h"
#include "auxiliars.h"
#include "math.h"
#include <vector>
#include "HataSRD.h"

vector<vector<double>> table;

double bit_error_probability(int env, int technology, int  bit_rate, int  transmitter_power, int  h_tx, int  h_rx, double  d, int SRD)
{

	double N, frequency, bandwidth, gross_bit_rate, bit_error, SNR, SNR_dB, gamma_b, gamma_b_dB;
	switch (env)
	{
	case Urbano:
		N = -82;
		break;
	case Suburbano:
		N = -88.5280;
		break;
	case Rural:
		N = -100.8168;
		break;
	default:
		break;
	}

	if (technology == t802_11_g)
	{
		frequency = 2400;
		bandwidth = 20 * (pow(10.0, 6.0));

		if ((bit_rate == 6) || (bit_rate == 9))
			gross_bit_rate = 12;
		else if ((bit_rate == 12) || (bit_rate == 18))
			gross_bit_rate = 24;
		else if ((bit_rate == 24) || (bit_rate == 36))
			gross_bit_rate = 48;
		else if ((bit_rate == 48) || (bit_rate == 54))
			gross_bit_rate = 72;


		double spectral_efficiency = gross_bit_rate*(pow(10.0, 6.0)) / bandwidth;

		double propagation_attenuation = loss(frequency, h_tx, h_rx, d, env, SRD);
		double received_power = transmitter_power - propagation_attenuation;

		SNR_dB = received_power - N;

		SNR = (pow(10.0, (SNR_dB / 10)));
		gamma_b = SNR / spectral_efficiency;
		gamma_b_dB = 10 * log10(gamma_b);


		//BPSK
		if (bit_rate == 6)
			bit_error = coded_modulation(bpsk, 1.0 / 2.0, sdd, gamma_b);

		else if (bit_rate == 9)
			bit_error = coded_modulation(bpsk, 3.0 / 4.0, sdd, gamma_b);

		//QPSK
		else if (bit_rate == 12)
			bit_error = coded_modulation(qpsk, 1.0 / 2.0, sdd, gamma_b);

		else if (bit_rate == 18)
			bit_error = coded_modulation(qpsk, 3.0 / 4.0, sdd, gamma_b);

		//16-QAM 
		else if (bit_rate == 24)
			bit_error = coded_modulation(t16_qam, 1.0 / 2.0, sdd, gamma_b);

		else if (bit_rate == 36)
			bit_error = coded_modulation(t16_qam, 3.0 / 4.0, sdd, gamma_b);

		//64-QAM
		else if (bit_rate == 48)
			bit_error = coded_modulation(t64_qam, 2.0 / 3.0, sdd, gamma_b);

		else if (bit_rate == 54)
			bit_error = coded_modulation(t64_qam, 3.0 / 4.0, sdd, gamma_b);

	}

	if (technology == t802_11_a)
	{

		frequency = 5800;
		bandwidth = 20 * (pow(10.0, 6.0));

		if ((bit_rate == 6) || (bit_rate == 9))
			gross_bit_rate = 12;

		else if ((bit_rate == 12) || (bit_rate == 18))
			gross_bit_rate = 24;

		else if ((bit_rate == 24) || (bit_rate == 36))
			gross_bit_rate = 48;

		else if ((bit_rate == 48) || (bit_rate == 54))
			gross_bit_rate = 72;


		double spectral_efficiency = gross_bit_rate*(pow(10.0, 6.0)) / bandwidth;

		double propagation_attenuation = loss(frequency, h_tx, h_rx, d, env, SRD);
		double received_power = transmitter_power - propagation_attenuation;

		SNR_dB = received_power - N;

		SNR = (pow(10.0, (SNR_dB / 10)));

		gamma_b = SNR / spectral_efficiency;
		gamma_b_dB = 10 * log10(gamma_b);

			//BPSK
		if (bit_rate == 6)
			bit_error = coded_modulation(bpsk, 1.0 / 2.0, sdd, gamma_b);

		else if (bit_rate == 9)
			bit_error = coded_modulation(bpsk, 3.0 / 4.0, sdd, gamma_b);

		//QPSK
		else if (bit_rate == 12)
			bit_error = coded_modulation(qpsk, 1.0 / 2.0, sdd, gamma_b);

		else if (bit_rate == 18)
			bit_error = coded_modulation(qpsk, 3.0 / 4.0, sdd, gamma_b);

		//16-QAM 
		else if (bit_rate == 24)
			bit_error = coded_modulation(t16_qam, 1.0 / 2.0, sdd, gamma_b);

		else if (bit_rate == 36)
			bit_error = coded_modulation(t16_qam, 3.0 / 4.0, sdd, gamma_b);

		//64-QAM
		else if (bit_rate == 48)
			bit_error = coded_modulation(t64_qam, 2.0 / 3.0, sdd, gamma_b);

		else if (bit_rate == 54)
			bit_error = coded_modulation(t64_qam, 3.0 / 4.0, sdd, gamma_b);

	}
	if (technology == t802_11_b)
	{


		frequency = 2400;
		bandwidth = 22 * (pow(10.0, 6.0));

		if (bit_rate == 1)
			gross_bit_rate = 1;

		else if (bit_rate == 2)
			gross_bit_rate = 2;

		else if (bit_rate == 5.5)
			gross_bit_rate = 5.5;

		else if (bit_rate == 11)
			gross_bit_rate = 11;


		double spectral_efficiency = gross_bit_rate*(pow(10.0, 6.0)) / bandwidth;

		double propagation_attenuation = loss(frequency, h_tx, h_rx, d, env, SRD);
		double received_power = transmitter_power - propagation_attenuation;


		SNR_dB = received_power - N;

		SNR = (pow(10.0, (SNR_dB / 10)));

		gamma_b = SNR / spectral_efficiency;
		gamma_b_dB = 10 * log10(gamma_b);


			//DBPSK
		if (bit_rate == 1)
			bit_error = uncoded_modulation(dbpsk, gamma_b);

		//DQPSK
		else if (bit_rate == 2)
			bit_error = uncoded_modulation(dqpsk, gamma_b);

		//CCK
		else if (bit_rate == 5.5)
			bit_error = uncoded_modulation(cck, gamma_b);

		//CCK
		else if (bit_rate == 11)
			bit_error = uncoded_modulation(cck, gamma_b);

	}

	if (technology == t802_15_4)
	{
		//frequency = 868;	%[MHz]
		//frequency = 915;	%[MHz]
		frequency = 2400;
		bandwidth = 2 * (pow(10.0, 6.0));

		double link_bit_rate = 250 * (1000);

		double spectral_efficiency = link_bit_rate / bandwidth;

		double propagation_attenuation = loss(frequency, h_tx, h_rx, d, env, SRD);
		double received_power = transmitter_power - propagation_attenuation;


		SNR_dB = received_power - N;


		SNR = (pow(10.0, (SNR_dB / 10)));
		gamma_b = SNR / spectral_efficiency;
		gamma_b_dB = 10 * log10(gamma_b);



			//BPSK
		if ((frequency == 868) || (frequency == 915))
			bit_error = uncoded_modulation(bpsk, gamma_b);
		//OQPSK +/-= QPSK
		else if ((frequency == 2400))
			bit_error = uncoded_modulation(oqpsk, gamma_b);

	}
	return bit_error;


}
double coded_modulation(int modulation_type, double code_rate, int decode_type, double gamma_b)
{

	int constraint = 9;

	double ber;




	if (code_rate == 1)
	{
		switch (modulation_type)
		{
		case bpsk:
		case qpsk:
		case dbpsk:
		case dqpsk:
		case oqpsk:
		case t16_qam:
		case t64_qam:
			ber = uncoded_modulation(modulation_type, gamma_b);
			break;
		default:
			break;
		}
	}
	// Coded
	else 
	{
		double d_free;
		vector<int> alpha_d;
		if (code_rate == 1.0 / 2.0)
		{
			d_free = 10;
			alpha_d = { 36, 0, 211, 0, 1404, 0, 11633, 0, 77433, 0 };
		}
		else if (code_rate == 2.0 / 3.0)
		{
			d_free = 6;
			alpha_d = { 3, 70, 285, 1276, 6160, 27128, 117019, 498860, 2103891, 8984123 };
		}
		else if (code_rate == 3.0 / 4.0)
		{
			d_free = 5;
			alpha_d = { 42, 201, 1492, 10469, 62935, 379644, 2253373, 13073811, 75152755, 428005675 };
		}

		switch (decode_type)
		{

		case hdd:
		case hard:
		{
			double P_e_HDD = 0;
			for (int d = 0; d < d_free + constraint; d++)
			{
				double P_b = uncoded_modulation(modulation_type, code_rate*gamma_b);
				double p_2 = 0;
				for (int k_ = 0; k_ < ceil((d + 1) / 2) / d; k_++)
					p_2 = p_2 + nchoosek(d, k_) * pow(P_b, k_) * pow((1 - P_b), (d - k_));
				if (ceil(d / 2) == d / 2)
					p_2 = p_2 + nchoosek(d, d / 2) * pow((P_b*(1 - P_b)), (d / 2)) / 2;
				P_e_HDD = P_e_HDD + alpha_d[d - d_free + 1] * p_2;
				ber = P_e_HDD;

			}
		}
			break;
		case soft:
		case sdd:
		{
			switch (modulation_type)
			{
			case bpsk:
			case qpsk:
				{
					double P_e_SDD = 0;

					for (int d = d_free; d < d_free + constraint; d++)
					{
						double p_2 = uncoded_modulation(modulation_type, code_rate*d*gamma_b);
						P_e_SDD = P_e_SDD + alpha_d[d - d_free + 1] * p_2;
					}

					ber = P_e_SDD;
				}
				break;

			case t16_qam:
				{
							double P_e_SDD_max = 0;
							double P_e_SDD_min = 0;
							for (int d = 0; d < d_free + constraint; d++)
							{
								double p_2_max = 0.5 * erfc(sqrt((2.0 / 5.0)*code_rate*d*gamma_b));
								double p_2_min = 0.5 * erfc(sqrt((18.0 / 5.0)*code_rate*d*gamma_b));
								P_e_SDD_max = alpha_d[d - d_free + 1] * p_2_min;
								P_e_SDD_min = alpha_d[d - d_free + 1] * p_2_max;

							}
							double P_e_SDD = 0.5 *(P_e_SDD_max + P_e_SDD_min);
							ber = P_e_SDD;
				}
				break;

			case t64_qam:
				{
							double P_e_SDD_max = 0;
							double P_e_SDD_min = 0;
							for (int d = 0; d < d_free + constraint; d++)
							{
								double p_2_max = 0.5 * erfc(sqrt((1.0 / 7.0)*code_rate*d*gamma_b));
								double p_2_min = 0.5 * erfc(sqrt((7.0) * code_rate*d*gamma_b));
								P_e_SDD_max = alpha_d[d - d_free + 1] * p_2_min;
								P_e_SDD_min = alpha_d[d - d_free + 1] * p_2_max;

							}
							double P_e_SDD = 0.5 *(P_e_SDD_max + P_e_SDD_min);
							ber = P_e_SDD;
				}
				break;

			default:
				break;
			}
			break;
		}
		default:
			break;

		}


	}



	if (ber > 0.5)
		ber = 0.5;
	//ber(ber>0.5) = 0.5;
	return ber;

}
double deviation(double d)
{

	double path_deviation;
	if (d <= 0.04)
		path_deviation = 3.5;
	else if ((0.04 < d) && (d <= 0.1))
		path_deviation = 3.5 + ((17 - 3.5)*(d - 0.04)) / (0.1 - 0.04);

	else if ((0.1 > d) && (d <= 0.2))
		path_deviation = 17;

	else if ((0.2 < d) && (d <= 0.6))
		path_deviation = 17 + ((9 - 17)*(d - 0.2)) / (0.6 - 0.2);
	else
		path_deviation = 9;
	return path_deviation;

}
double  fading_modulation(int modulation_type, double gamma_b, double diversity_order)
{

	double ber;
	if (modulation_type == bpsk)
	{
		double K = log2(2);
		double gama_c = (gamma_b*K) / diversity_order;

		double mu = sqrt(gama_c / (1 + gama_c));
		ber = pskf(mu, diversity_order);
	}

	if (ber>0.5)
		ber = 0.5;
	return ber;

}

double pskf(double mu, double divOrder){
	//????????????????

	double out = 0;
	for (int i = 0; i < divOrder - 1; i++)
		out = out + nchoosek(2 * i, i) * pow(((pow(1 - mu, 2)) / 4), i);

	out = (1 - mu*out) / 2;

	return 0;
}
double loss(double f, double h_tx, double h_rx, double d, int environment, int SRD)
{

	double path_loss = 0;

	double alpha = 1;

	double H_m = fmin(h_tx, h_rx);
	double H_b = fmax(h_tx, h_rx);

	double a = (1.1*log10(f) - 0.7)*fmin(10, H_m) - (1.56*log10(f) - 0.8) + fmax(0, 20 * log10(H_m / 10));
	double b = 0;
	//if (!SRD)
		//b = Math.min(0.20*log10(H_b / 30));
	if (SRD)
		b = (((1.1*log10(f)) - 0.7)*fmin(10, H_b)) - (((1.56*log10(f)) - 0.8)) + fmax(0, (20 * log10(H_b / 10)));
	if (d <= 0.04)
		path_loss = 32.4 + 20 * log10(f) + 10 * log10((pow(d, 2)) + ((((pow((H_b - H_m), 2))) / pow(10, 6))));

	if (d >= 0.1)
	{


		switch (environment)
		{

		case Urbano:

			if ((30 <= f) && (f <= 150))
				path_loss = 69.9 + 26.2*log10(150) - 20 * log10(150 / f) - 13.82*log10(fmax(30, H_b)) + (44.9 - 6.55*log10(fmax(30, H_b)))*(pow(log10(d), alpha)) - a - b;
			if ((150 <  f) && (f <= 1500))
				path_loss = 69.6 + 26.2*log10(f) - 13.82*log10(fmax(30, H_b)) + (44.9 - 6.55*log10(fmax(30, H_b)))*(pow(log10(d), alpha)) - a - b;

			if ((1500 < f) && (f <= 2000))
				path_loss = 46.3 + 33.9*log10(f) - 13.82*log10(fmax(30, H_b)) + (44.9 - 6.55*log10(fmax(30, H_b)))*(pow(log10(d), alpha)) - a - b;

			if ((2000 < f) && (f <= 3000))
				path_loss = 46.3 + 33.9*log10(2000) + 10 * log10(f / 2000) - 13.82*log10(fmax(30, H_b)) + (44.9 - 6.55*log10(fmax(30, H_b))) * (pow(log10(d), alpha)) - a - b;

			break;

		case Suburbano:
			path_loss = loss(f, h_tx, h_rx, d, Urbano, SRD) - 2 * (pow(log10(fmin(fmax(150, f), 2000)) / 28, 2)) - 5.4;
			break;

		case Rural:
			path_loss = loss(f, h_tx, h_rx, d, Urbano, SRD) - 4.78*(pow(log10(fmin(fmax(150, f), 2000)), 2)) + 18.33*log10(fmin(fmax(150, 2400), 2000)) - 20.94;
			break;

		default:
			break;

		}

	}


	if ((0.04 < d) && (d < 0.1)){
		double l_01 = loss(f, h_tx, h_rx, 0.1, environment, SRD);
		double l_004 = loss(f, h_tx, h_rx, 0.04, environment, SRD);
		path_loss = l_004 + (((log10(d)) - log10(0.04)) / (log10(0.1) - log10(0.04))) * (l_01 - l_004);
	}
	return path_loss;
}
int nchoosek(int n, int k)
{
	return fac(n) / (fac(n - k)*fac(k));
}
int fac(int n)
{
	if (n <= 1)
		return 1;
	int ret = 1;
	while (n != 1)
	{
		ret *= n;
		n--;
	}

}
double uncoded_modulation(int modulation_type, double gamma_b)
{


	double ber,a,b;


	switch (modulation_type){
	case oqpsk:
	case qpsk:
	case bpsk:
		ber = 0.5 * erfc(sqrt(gamma_b));
		break;

	case dbpsk:
		ber = 0.5 * exp(-gamma_b);
		break;

	case dqpsk:
		//a = sqrt((2 * gamma_b)*(1 - sqrt(0.5)));
		//b = sqrt((2 * gamma_b)*(1 + sqrt(0.5)));
		//ber = marcumq(a, b) - 0.5*besseli(0, a*b)*exp(-0.5*(a*a + b*b));
		break;

	case t16_qam:
		ber = 3 / 8 * erfc(sqrt(2 / 5 * gamma_b))
			+ 1 / 4 * erfc(3 * sqrt(2 / 5 * gamma_b))
			- 1 / 8 * erfc(5 * sqrt(2 / 5 * gamma_b));
		break;

	case t64_qam:
		ber = 7 / 24 * erfc(sqrt(1 / 7 * gamma_b))
			+ 1 / 4 * erfc(3 * sqrt(1 / 7 * gamma_b))
			- 1 / 24 * erfc(5 * sqrt(1 / 7 * gamma_b))
			+ 1 / 24 * erfc(9 * sqrt(1 / 7 * gamma_b))
			- 1 / 24 * erfc(13 * sqrt(1 / 7 * gamma_b));
		break;

	case cck:

		//var X = Math.sqrt(2 * gamma_b);

		//var M = 4;
		/*
		syms v;
		syms y;

		fun = @(v,y) ((exp(-0.5*(y.^2))/sqrt(2*pi)).^((M/2)-1)).*exp(-0.5*(v.^2));
		ymax = @(v)  (v+X);
		ymin = @(v) -(v+X);
		p_o = integral2(fun,-X,Inf,ymin,ymax);

		ber = 1 - p_o;
		*/
		break;


	}
	if (ber>0.5)
		ber = 0.5;
	return ber;

}
//double getHataSRDSuccessRate(double distance, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD)
//{
//	return 1 - bit_error_probability(env, technology, bit_rate, transmitter_power,  h_tx,  h_rx,  distance/1000, SRD);
//	
//
//}
double getHataSRDSuccessRate(double distance, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD)
{
	if (env == Suburbano)
	{
		if (distance <= 49)
			return 1;
		else
			return 0;

	}
	if (env == Urbano)
	{
		if (distance <= 23)
			return 1;
		else
			return 0;

	}
		


	int distAprox = ceil(distance);
	if (distAprox == 0)
		return 1;
	else if (distAprox > table.size())
		return 0;
	else
	{
		switch (env)
		{
			case Urbano:
				return table[distAprox-1][2];
				break;
			case Suburbano:
				return table[distAprox - 1][1];
				break;
			case Rural:
				return table[distAprox - 1][0];
				break;
			default:
				return 0;
				break;
		}
	}





}
void propagationTable()
{

	FILE *file;
	vector<vector<double>> ret;

	fopen_s(&file, "dadossiris.txt", "r");
	if (file)
	{
		double rur = 0;
		double subur = 0;
		double urb = 0;
		int size = 0;
		fscanf_s(file, "%d", &size);
		for (int i = 0; i < size; i++)
		{
			vector<double> toAdd;
			fscanf_s(file, "%lf %lf %lf", &rur, &subur, &urb);

			toAdd.push_back(rur);
			toAdd.push_back(subur);
			toAdd.push_back(urb);
			ret.push_back(toAdd);
			toAdd.clear();
		}
		fclose(file);
	}

	
	table = ret;
	
}



