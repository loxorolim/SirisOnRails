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

const string helpMessage = "\n -i : Arquivo .kml de entrada. Ex: Input.kml \n -o : Arquivo .kml de saida. Ex: Output.kml \n -t : Tecnologia considerada. \n \t0: 802.11g \n \t1: ZigBee \n -s : Cenario considerado: \n \t0: Urbano \n \t1: Suburbano \n \t2: Rural \n -p : Potencia dos dispositivos \n -m : Numero de saltos mesh \n -h : Esta mensagem de ajuda";
int tech = t802_11_g, scenario = Urbano, power = 20, meshHops = 0;
string inputFile = "", outputFile = "";

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
int setTechnology(char* cmd, string& err)
{
	if (is_number(cmd))
	{
		int val = atoi(cmd);
		if (val == t802_11_g || val == t802_15_4)
			tech = val;
		else
		{
			err = " ERRO: Valor de tecnologia desconhecida. ";
			return 1;
		}
	}
	else
	{
		err = " ERRO: Digite um numero no valor de tecnologia! ";
		return 1;
	}
}
int setScenario(char* cmd, string& err)
{
	if (is_number(cmd))
	{
		int val = atoi(cmd);
		if (val == Urbano || val == Suburbano || val == Rural)
		{
			scenario = val;
			return 0;
		}
		else
		{
			err += " ERRO: Valor de cenario desconhecido. ";
			return 1;
		}
	}
	else
	{
		err += " ERRO: Digite um numero no valor de cenario!";
		return 1;
	}
}
int setPower(char* cmd, string& err)
{
	if (is_number(cmd))
	{
		int	val = atoi(cmd);
		power = val;
		if (val < -10 || val > 30)
		{
			err += " ALERTA: Valor de potencia esta muito alto ou muito baixo. ";
		}
		return 0;
	}
	else
	{
		err = " ERRO: Digite um numero no valor de potencia!";
		return 1;
	}
}
int setMeshHops(char* cmd, string& err)
{
	if (is_number(cmd))
	{
		int	val = atoi(cmd);
		if (val < 0)
		{
			err += " ERRO: O valor de saltos mesh deve ser superior ou igual a 0! ";
			return 1;
		}
		else
		{
			meshHops = val;
			if (val > 4)
				err += " ALERTA: Valores superiores a 4 saltos mesh podem comprometer a veracidade dos resultados!";
			return 0;
		}
		
	}
	else
	{
		err = " ERRO: Digite um numero no valor de saltos mesh!";
		return 1;
	}
}
int setInputFile(char* cmd, string& err)
{
	if (cmd == "")
	{
		err = " Nome de arquivo de input vazio.";
		return 1;
	}
	inputFile = cmd;
	return 0;
}
int setOutputFile(char* cmd, string& err)
{
	if (cmd == "")
	{
		err = " Nome de arquivo de output vazio.";
		return 1;
	}
	outputFile = cmd;
	return 0;
}

int main(int argc, char* argv[])
{
	int e=0;
	for (int i = 1; i < argc; i++)
	{
		string input = argv[i];
		if (input == "-i")
		{
			string err = "";
			cout << argv[i + 1];
			e = setInputFile(argv[i + 1],err);
			cout << "\n" << err;
			i++;
		}
		else if (input == "-o")
		{
			string err = "";
			e = setOutputFile(argv[i + 1], err);
			cout << "\n" << err;
			i++;
		}
		else if (input == "-t")
		{
			string err = "";
			e = setTechnology(argv[i + 1], err);
			cout << "\n" << err;
			i++;
		}
		else if (input == "-s")
		{
			string err = "";
			e = setScenario(argv[i + 1], err);
			cout << "\n" << err;
			i++;
		}
		else if (input == "-p")
		{
			string err = "";
			e = setPower(argv[i + 1], err);
			cout << "\n" << err;
			i++;
		}
		else if (input == "-m")
		{
			string err = "";
			e = setMeshHops(argv[i + 1], err);
			cout << "\n" << err;
			i++;
		}
		else if (input == "-h")
		{
			cout << helpMessage;
		}
		else
		{
			cout << "\n Comando nao identificado: " << input;
		}
	}
	if (e)
		return 0;
	else
	{
		//executa;
	}
		
	
	return 0;

}