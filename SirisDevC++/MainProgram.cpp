#include <iostream>
#include <sstream>
#include "Grid.h"
#include "auxiliars.h"
#include "AutoPlanningMethods.h"
#include "LinkCalculationMethods.h"
#include "MetricCalculationMethods.h"
#include "KMLMethods.h"
#include "HataSRD.h"
#include <stdio.h>

#define TECHNOLOGY_DEFAULT t802_11_g
#define T80211G_BIT_RATE_DEFAULT 6
#define T80211G_POWER_DEFAULT 20
#define T802154_BIT_RATE_DEFAULT 0.250
#define T802154_POWER_DEFAULT 0
#define SCENARIO_DEFAULT Urbano
#define MESH_HOPS_DEFAULT 0
#define H_TX_DEFAULT 3
#define H_RX_DEFAULT 5
#define REDUNDANCY_DEFAULT 1


const string INPUT_CMD = "-i -input -I";
const string OUTPUT_CMD = "-o -output -O";
const string SCENARIO_CMD = "-s -scenario -S";
const string TECHNOLOGY_CMD = "-t -technology -T";
const string POWER_CMD = "-p -power -P";
const string MESH_HOPS_CMD = "-m -meshhops -M";
const string HELP_CMD = "-h --h -help";
const string REDUNDANCY_CMD = "-r -redundancy -R";
const string HTX_CMD = "-htx";
const string HRX_CMD = "-hrx";
const string BIT_RATE_CMD = "-b -bitrate";
const string TIME_LIMIT_CMD = "-tmlim";
const string MEM_LIMIT_CMD = "-memlim";

const string helpMessage = "\n " + INPUT_CMD + " : Arquivo .kml de entrada. Ex: Input.kml \n " + OUTPUT_CMD + " : Arquivo .kml de saida. Ex: Output.kml \n " + TECHNOLOGY_CMD + " : Tecnologia considerada. \n \t0: 802.11g \n \t1: ZigBee \n " + SCENARIO_CMD + " : Cenario considerado. \n \t" + to_string(Urbano) + ": Urbano \n \t" + to_string(Suburbano) + ": Suburbano \n \t" + to_string(Rural) + ": Rural \n " + POWER_CMD + " : Potencia dos dispositivos \n " + MESH_HOPS_CMD + " : Numero de saltos mesh \n " + REDUNDANCY_CMD + " : Redundancia de cobertura para cada medidor \n " + HTX_CMD + " : Altura dos medidores em metros \n " + HRX_CMD + " : Altura dos agregadores em metros\n " + BIT_RATE_CMD + " : Taxa de transmissao dos dispositivos em megabits por segundo \n " + HELP_CMD + " : Esta mensagem de ajuda";
int tech = TECHNOLOGY_DEFAULT, scenario = SCENARIO_DEFAULT, power = T80211G_POWER_DEFAULT, meshHops = MESH_HOPS_DEFAULT, redundancy = REDUNDANCY_DEFAULT;
double h_tx = H_TX_DEFAULT, h_rx = H_RX_DEFAULT, bit_rate = T802154_BIT_RATE_DEFAULT;
string inputFile = "", outputFile = "";

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
bool is_cmd(char* cmd, string str)
{
	vector<string> possibilites = split(str, ' ');
	for (int i = 0; i < possibilites.size(); i++)
	{
		if (cmd == possibilites[i])
			return true;
	}
	return false;
}

int getInputFileOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i],INPUT_CMD))
		{
			inputFile = argv[i + 1];
			return 0;
		}
	}
	cerr << "\nInsira um arquivo de entrada!";
	return 1;
}
int getOutputFileOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i],OUTPUT_CMD))
		{
			outputFile = argv[i + 1];
			return 0;
		}
	}
	cerr << "\nInsira um arquivo de saida!";
	return 1;
}
int getScenarioOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], SCENARIO_CMD))
		{
			if (is_number(argv[i+1]))
			{
				int val = atoi(argv[i + 1]);
				if (val == Urbano || val == Suburbano || val == Rural)
				{
					scenario = val;
					return 0;
				}
				else
				{
					cerr << "\nERRO: Valor de cenario desconhecido. ";
					return 1;
				}
			}
			else
			{
				cerr << "\nERRO: Digite um numero no valor de cenario!";
				return 1;
			}
		}
	}
	return 0;
}
int getTechnologyOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], TECHNOLOGY_CMD))
		{
			if (is_number(argv[i+1]))
			{
				int val = atoi(argv[i + 1]);
				if (val == t802_11_g || val == t802_15_4)
					tech = val;
				else
				{
					cerr << "\nERRO: Valor de tecnologia desconhecida. ";
					return 1;
				}
			}
			else
			{
				cerr << "\nERRO: Digite um numero no valor de tecnologia! ";
				return 1;
			}
		}
	}
	return 0;
}
int getPowerOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], POWER_CMD))
		{
			if (is_number(argv[i+1]))
			{
				int	val = atoi(argv[i + 1]);
				power = val;
				if (val < -10 || val > 30)
				{
					cerr << "\nALERTA: Valor de potencia esta muito alto ou muito baixo. ";
				}
				return 0;
			}
			else
			{
				cerr << "\nERRO: Digite um numero no valor de potencia!";
				return 1;
			}
		}
	}
	return 0;
}
int getTimeLimitOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], TIME_LIMIT_CMD))
		{
			if (is_number(argv[i + 1]))
			{
				int	val = atoi(argv[i + 1]);
				TIME_LIMIT = val;
				return 0;
			}
			else
			{
				cerr << "\nERRO: Digite um numero no valor de potencia!";
				return 1;
			}
		}
	}
	return 0;
}
int getMemLimitOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], MEM_LIMIT_CMD))
		{
			if (is_number(argv[i + 1]))
			{
				int	val = atoi(argv[i + 1]);
				MEM_LIMIT = val;
				return 0;
			}
			else
			{
				cerr << "\nERRO: Digite um numero no valor de potencia!";
				return 1;
			}
		}
	}
	return 0;
}
int getRedundancyOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], REDUNDANCY_CMD))
		{
			if (is_number(argv[i + 1]))
			{
				int	val = atoi(argv[i + 1]);
				if (val > 1)
					redundancy = val;
				else
				{
					cerr << "\nERRO: Valor deve ser maior ou igual a 1. ";
					return 1;
				}
				if (val > 3)
				{
					cerr << "\nALERTA: Este valor de redundancia e alto e pode comprometer o resultado. ";
				}
				return 0;
			}
			else
			{
				cerr << "\nERRO: Digite um numero no valor de redundancia!";
				return 1;
			}
		}
	}
	return 0;
}
int getHTXOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], HTX_CMD))
		{
			if (is_number(argv[i + 1]))
			{
				int	val = atoi(argv[i + 1]);
				if (val >= 0)
					h_tx = val;
				else
				{
					cerr << "\nERRO: Valor deve ser maior ou igual a 0. ";
					return 1;
				}
				return 0;
			}
			else
			{
				cerr << "\nERRO: Digite um numero no valor de altura dos medidores!";
				return 1;
			}
		}
	}
	return 0;
}
int getHRXOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], HRX_CMD))
		{
			if (is_number(argv[i + 1]))
			{
				int	val = atoi(argv[i + 1]);
				if (val >= 0)
					h_rx = val;
				else
				{
					cerr << "\nERRO: Valor deve ser maior ou igual a 0. ";
					return 1;
				}
				return 0;
			}
			else
			{
				cerr << "\nERRO: Digite um numero no valor de altura dos medidores!";
				return 1;
			}
		}
	}
	return 0;
}
int getBitRateOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], BIT_RATE_CMD))
		{
			if (is_number(argv[i + 1]))
			{
				int	val = atoi(argv[i + 1]);
				if (val >= 0)
					bit_rate = val;
				else
				{
					cerr << "\nERRO: Valor deve ser maior ou igual a 0. ";
					return 1;
				}
				return 0;
			}
			else
			{
				cerr << "\nERRO: Digite um numero no valor de altura dos medidores!";
				return 1;
			}
		}
	}
	return 0;
}
int getMeshHopsOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], MESH_HOPS_CMD))
		{
			if (is_number(argv[i+1]))
			{
				int	val = atoi(argv[i + 1]);
				if (val < 0)
				{
					cerr << "\nERRO: O valor de saltos mesh deve ser superior ou igual a 0! ";
					return 1;
				}
				else
				{
					meshHops = val;
					if (val > 4)
						cerr << "\nALERTA: Valores superiores a 4 saltos mesh podem comprometer a veracidade dos resultados!";
					return 0;
				}

			}
			else
			{
				cerr << "\nERRO: Digite um numero no valor de saltos mesh!";
				return 1;
			}
		}
	}
	return 0;
}
void getHelpMessageOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], HELP_CMD))
		{
			cout << helpMessage;
		}
	}
}
void printPlanningResume(int mSize, int pSize, int sSize)
{
	cout << "\nIniciando planejamento com as seguintes configuracoes:";
	cout << "\nNumero de medidores: " << to_string(mSize);
	cout << "\nNumero de postes: " << to_string(pSize);
	cout << "\nNumero de pontos de coleta de sinal: " << to_string(sSize);
	cout << "\nCenario: " + scenario_map.at(scenario);
	cout << "\nTecnologia: " + technology_map.at(tech);
	cout << "\nPotencia dos dispositivos: " + to_string(power) ;
	cout << "\nTaxa de transmissao: " + to_string(bit_rate) + " mbps";
	cout << "\nAltura dos medidores: " + to_string(h_tx) + " m";
	cout << "\nAltura dos agregadores: " + to_string(h_rx)+" m";
	cout << "\nNumeros de saltos maximos: " + to_string(meshHops+1) + ", onde "+ to_string(meshHops)+" sao saltos Mesh";
	cout << "\nResultado sera gravado em: " << outputFile;
	
}
int main(int argc, char* argv[])
{
	//convertMeterAndPolesToKml("C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\SirisOnRails\\arqsTeste\\filemeters9999999.txt", "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\SirisOnRails\\arqsTeste\\filepoles9999999.txt", "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\SirisOnRails\\arqsTeste\\Floripa.kml");

	int e = 0;
	e += getInputFileOption(argc, argv);
	e += getOutputFileOption(argc, argv);
	e += getTechnologyOption(argc, argv);
	switch (tech)
	{
		case t802_11_g:
			power = T80211G_POWER_DEFAULT;
			bit_rate = T80211G_BIT_RATE_DEFAULT;
			break;
		case t802_15_4:
			power = T802154_POWER_DEFAULT;
			bit_rate = T802154_BIT_RATE_DEFAULT;
			break;
		default:
			break;

	}
	e += getScenarioOption(argc, argv);
	e += getPowerOption(argc, argv);
	e += getMeshHopsOption(argc, argv);
	e += getTimeLimitOption(argc, argv);
	e += getMemLimitOption(argc, argv);
	getHelpMessageOption(argc, argv);
	if (e)
		return 0;
	else
	{
		vector<Position*> daps, meters, poles, coverageArea;
		int e_read = readKML(inputFile.c_str(), daps,meters,poles,coverageArea);
		if (!e_read)
		{
			printPlanningResume(meters.size(), poles.size(), coverageArea.size());
			AutoPlanning* res = new AutoPlanning(meters, poles, scenario, tech, bit_rate, power, h_tx, h_rx, 1, meshHops, 500, "");
			string ret = res->clusterAutoPlanning(true, redundancy);
			vector<string> chosenDaps = split(ret, ' ');
			for (int i = 0; i < chosenDaps.size(); i++)
			{
				int pos = stoi(chosenDaps[i]);
				double lat = poles[pos]->latitude;
				double lng = poles[pos]->longitude;
				Position *newDap = new Position(lat, lng, daps.size());
				daps.push_back(newDap);
			}
			KMLMethods* kmethods = new KMLMethods(meters, daps, poles, coverageArea,scenario, tech, bit_rate, power, h_tx, h_rx, 1, meshHops, "");
			kmethods->saveKmlToFile(outputFile);
			delete kmethods;
		}
	}
		
	
	return 0;

}