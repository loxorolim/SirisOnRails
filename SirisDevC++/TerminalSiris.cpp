#include <iostream>
#include <sstream>
#include "Grid.h"
#include "auxiliars.h"
#include "AutoPlanningMethods.h"
#include "LinkCalculationMethods.h"
#include "MetricCalculationMethods.h"
#include "KMLMethods.h"
#include "HataSRD.h"
#include "MainMethods.h"
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
#define VALID_CELL_RADIUS_DEFAULT 20


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
const string VALID_CELL_RADIUS_CMD = "-vcr";
const string CONSIDER_CELL_RADIUS_CMD = "-ccr";
const string VERBOSE_CMD = "-verbose -v -V";
const string OVERWRITE_CMD = "-ow -overwrite -OW";

const string helpMessage = "\n " + INPUT_CMD + " : Arquivo .kml de entrada. Ex: Input.kml \n " + 
OUTPUT_CMD + " : Arquivo .kml de saida. Ex: Output.kml \n " + 
TECHNOLOGY_CMD + " : Tecnologia considerada. \n \t0: 802.11g \n \t1: ZigBee \n " + 
SCENARIO_CMD + " : Cenario considerado. \n \t" + to_string(Urbano) + ": Urbano \n \t" + to_string(Suburbano) + ": Suburbano \n \t" + to_string(Rural) + ": Rural \n " + 
POWER_CMD + " : Potencia dos dispositivos (Numero real) \n " + 
MESH_HOPS_CMD + " : Numero de saltos mesh (Inteiro) \n " + 
REDUNDANCY_CMD + " : Redundancia de cobertura para cada medidor (Inteiro)\n " + 
HTX_CMD + " : Altura dos medidores em metros (Numero real)\n " + 
HRX_CMD + " : Altura dos agregadores em metros (Numero real)\n " + 
BIT_RATE_CMD + " : Taxa de transmissao dos dispositivos em megabits por segundo (Numero real) \n " + 
TIME_LIMIT_CMD + " : Tempo limite para o solver resolver cada sub-instancia (Numero real) \n " + 
MEM_LIMIT_CMD + " : Memoria maxima limite para o metodo exato (Numero real) \n " + 
CONSIDER_CELL_RADIUS_CMD + " : Planejar considerando apenas os postes que possuem sinal 3G/4G/GPRS \n " + 
VALID_CELL_RADIUS_CMD + " : Alcance de efeito de cada ponto de sinal 3G/4G/GPRS (Inteiro) \n " + 
VERBOSE_CMD + " : Ativa modo verboso \n " +
OVERWRITE_CMD + " : Ativa sobrescrita de DAPs ja posicionados\n " + 
HELP_CMD + " : Esta mensagem de ajuda";
int tech = TECHNOLOGY_DEFAULT, scenario = SCENARIO_DEFAULT, power = T80211G_POWER_DEFAULT, meshHops = MESH_HOPS_DEFAULT, redundancy = REDUNDANCY_DEFAULT, valid_cell_radius = VALID_CELL_RADIUS_DEFAULT;
double h_tx = H_TX_DEFAULT, h_rx = H_RX_DEFAULT, bit_rate = T802154_BIT_RATE_DEFAULT;
bool verbose = false, ccr = false, overwrite = false;
string inputFile = "", outputFile = "";

bool check_if_can_write(string filename)
{
	ofstream f(filename.c_str());
	int e = f.rdstate();
	f.close();
	if (e)
		return false;
	return true;
}
bool is_number(const std::string& s)
{
	try
	{
		double value = std::stod(s);
		return true;
	}
	catch (std::exception& e)
	{
		return false;
	}
//	std::string::const_iterator it = s.begin();
//	while (it != s.end() && isdigit(*it)) ++it;
//	return !s.empty() && it == s.end();
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
				if (val > 0)
				{
					TIME_LIMIT = val;
					return 0;
				}
				else
				{
					cerr << "\nERRO: Valor para o tempo limite deve ser superior a 0!";
					return 1;
				}
			}
			else
			{
				cerr << "\nERRO: Digite um numero para o tempo limite!";
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
				if (val > 0)
				{
					MEM_LIMIT = val;
					return 0;
				}
				else
				{
					cerr << "\nERRO: Valor para o limite de memoria deve ser maior que 0. ";
					return 1;
				}

			}
			else
			{
				cerr << "\nERRO: Digite um numero para o limite de memoria!";
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
					cerr << "\nERRO: Valor de redundancia deve ser maior ou igual a 1. ";
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
					cerr << "\nERRO: Valor de altura dos medidores deve ser maior ou igual a 0. ";
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
					cerr << "\nERRO: Valor de altura dos agregadores deve ser maior ou igual a 0. ";
					return 1;
				}
				return 0;
			}
			else
			{
				cerr << "\nERRO: Digite um numero no valor de altura dos agregadores!";
				return 1;
			}
		}
	}
	return 0;
}
int getValidCellRadiusOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], VALID_CELL_RADIUS_CMD))
		{
			if (is_number(argv[i + 1]))
			{
				int	val = atoi(argv[i + 1]);
				if (val >= 1)
					valid_cell_radius = val;
				else
				{
					cerr << "\nERRO: Valor para a area de efeito dos pontos de sinal deve ser maior ou igual a 1. ";
					return 1;
				}
				return 0;
			}
			else
			{
				cerr << "\nERRO: Digite um numero para a area de efeito dos pontos de sinal 3G/4G/GPRS!";
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
bool getHelpMessageOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], HELP_CMD))
		{
			cout << helpMessage;
			return true;
		}
	}
	return false;
}
void getVerboseOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], VERBOSE_CMD))
		{
			verbose = true;
		}
	}
}
void getConsiderCellRadiusOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], CONSIDER_CELL_RADIUS_CMD))
		{
			ccr = true;
		}
	}
}
void getOverwriteOption(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd(argv[i], OVERWRITE_CMD))
		{
			overwrite = true;
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
	cout << "\nConsiderar apenas postes com sinal 3G/4G/GPRS: " << ccr;
	if (ccr)
		cout << "\nArea de efeito de um ponto de sinal 3G/4G/GPRS: " + to_string(valid_cell_radius);
	cout << "\nResultado sera gravado em: " << outputFile;
	cout << "\nLimite de memoria por instancia: " << MEM_LIMIT;
	cout << "\nLimite de tempo por instancia: " << TIME_LIMIT;
	
}
vector<Position*> selectPolesWithSignal(vector<Position*> &poles, vector<Position*> &coverage_area)
{
	vector<Position*> aux;
	for (int i = 0; i < poles.size(); i++)
	{
		for (int j = 0; j < coverage_area.size(); j++)
		{
			double dist = getDistance(poles[i], coverage_area[j]);
			if (dist <= valid_cell_radius) 
			{
				Position* p = new Position(coverage_area[j]->latitude, coverage_area[j]->longitude, aux.size(), coverage_area[j]->signalInfo);
				aux.push_back(p);
				break;
			}
		}
	}
	return aux;
	
}
int TerminalMain(int argc, char* argv[])
{
	//convertMeterAndPolesToKml("C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\arqsTeste\\filemeters9999999.txt", "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\arqsTeste\\filepoles9999999.txt", "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\arqsTeste\\Floripa.kml");

	int e = 0;
	if (getHelpMessageOption(argc, argv))
		return 0;
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
	e += getValidCellRadiusOption(argc, argv);
	getConsiderCellRadiusOption(argc, argv);
	getVerboseOption(argc, argv);
	getOverwriteOption(argc, argv);
	
	cout << "\n";
	if (e)
		return 0;
	else
	{
		
		if (!check_if_can_write(outputFile.c_str()))
		{
			cerr << "\nNao e possivel escrever no arquivo de saida";
			return 0;
		}
		vector<Position*> daps, meters, poles, coverageArea;
		if (verbose) cout << "\nLendo KML de entrada";
		int e_read = readKML(inputFile.c_str(), daps,meters,poles,coverageArea);		
		if (!e_read)
		{
			vector<Position*> aux_poles;
			if (ccr)
			{
				aux_poles = selectPolesWithSignal(poles, coverageArea);
			}
			if (verbose) cout << "\nKML de entrada lido com sucesso";
			printPlanningResume(meters.size(), poles.size(), coverageArea.size());
			AutoPlanning* res = new AutoPlanning(meters, aux_poles, scenario, tech, bit_rate, power, h_tx, h_rx, 1, meshHops, 500, "",verbose);
			vector<int> chosenDaps = res->clusterAutoPlanning(true, redundancy);
			string ret = "";
			for (int i = 0; i < chosenDaps.size(); i++)
				ret += to_string(chosenDaps[i]) + " ";
			//string ret = res->graspAutoPlanning(1000, 0.9);
			cout << "\n"+ret;
			//vector<string> chosenDaps = split(ret, ' ');
			if (overwrite)
			{
				for (int i = 0; i < daps.size(); i++)
					delete daps[i];
				daps.clear();
			}
			for (int i = 0; i < chosenDaps.size(); i++)
			{
				int pos = chosenDaps[i];
				double lat = aux_poles[pos]->latitude;
				double lng = aux_poles[pos]->longitude;
				Position *newDap = new Position(lat, lng, daps.size());
				daps.push_back(newDap);
			}
			KMLMethods* kmethods = new KMLMethods(meters, daps, poles, coverageArea,scenario, tech, bit_rate, power, h_tx, h_rx, 1, meshHops,valid_cell_radius, "",verbose);
			kmethods->saveKmlToFile(outputFile);
			delete kmethods;
			for (int i = 0; i < aux_poles.size(); i++)
				delete aux_poles[i];
		}
	}		
	return 0;
}
