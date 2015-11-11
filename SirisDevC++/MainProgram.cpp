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

const string helpMessage = "\n -i : Arquivo .kml de entrada. Ex: Input.kml \n -o : Arquivo .kml de saida. Ex: Output.kml \n -t : Tecnologia considerada. \n \t0: 802.11g \n \t1: ZigBee \n -s : Cenario considerado: \n \t0: Urbano \n \t1: Suburbano \n \t2: Rural \n -p : Potencia dos dispositivos \n -m : Numero de saltos mesh \n -h : Esta mensagem de ajuda";
int tech = t802_11_g, scenario = Urbano, power = 20, meshHops = 0;
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
	cout << helpMessage;
}
void printPlanningResume(int mSize, int pSize, int sSize)
{
	cout << "\nIniciando planejamento com as seguintes configuracoes:";
	cout << "\nNumero de medidores: " << to_string(mSize);
	cout << "\nNumero de postes: " << to_string(pSize);
	cout << "\nNumero de pontos de coleta de sinal: " << to_string(sSize);
	cout << "\nTecnologia: " + technology_map.at(tech);
	cout << "\nCenario: " + scenario_map.at(scenario);
	cout << "\nPotencia dos dispositivos: " + to_string(power) ;
	cout << "\nNumeros de saltos maximos: " + to_string(meshHops+1) + ",onde "+ to_string(meshHops)+" sao saltos Mesh";
	cout << "\nResultado sera gravado em: " << outputFile;
	
}
int main(int argc, char* argv[])
{
	int e = 0;
//	e += getInputFileOption(argc, argv);
//	e += getOutputFileOption(argc, argv);
	e += getScenarioOption(argc, argv);
	e += getTechnologyOption(argc, argv);
	e += getPowerOption(argc, argv);
	e += getMeshHopsOption(argc, argv);
	getHelpMessageOption(argc, argv);
	if (e)
		return 0;
	else
	{
		vector<Position*> daps, meters, poles, coverageArea;
		int e_read = readKML("C:\\Users\\Guilherme\\Downloads\\viz29-0-10.kml", daps,meters,poles,coverageArea);
		if (!e_read)
		{
			printPlanningResume(meters.size(), poles.size(), coverageArea.size());
			AutoPlanning* res = new AutoPlanning(meters, poles, scenario, tech, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, meshHops, rubyPath);
			string ret = res->executeAutoPlan(redundancy, limit);
		}
	}
		
	
	return 0;

}