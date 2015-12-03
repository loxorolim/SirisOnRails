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

////
//#include "rice/Class.hpp"
///* run this program using the console pauser or add your own getch, system("pause") or input loop */
//
//using namespace Rice;
using namespace rapidjson;

vector<Position*> getPositionArrayFromJson(string json, string type)
{
	Document d;
	d.Parse(json.c_str());
	const Value& aux = d[type.c_str()];
	vector<Position*> ret;
	for (rapidjson::SizeType i = 0; i < aux.Size(); i++)
	{
		double lat, lng;
		lat = aux[i]["lat"].GetDouble();
		lng = aux[i]["lng"].GetDouble();
		Position* p = new Position(lat, lng, i);
		ret.push_back(p);
	}
	return ret;
}
string getResponse(string input, string rP)
{
	Document d;

	d.Parse(input.c_str());
	int action_id, scenario, technology, mesh = 0, redundancy = 1;
	double h_tx, h_rx, bit_rate, power;
	// 2. Modify it by DOM.
	Value& s = d["action_id"];
	action_id = s.GetInt();
	s = d["scenario"];
	scenario = s.GetInt();
	s = d["technology"];
	technology = s.GetInt();
	s = d["redundancy"];
	redundancy = s.GetInt();
	s = d["mesh_hops"];
	mesh = s.GetInt();
	s = d["h_tx"];
	h_tx = s.GetDouble();
	s = d["h_rx"];
	h_rx = s.GetDouble();
	s = d["rate"];
	bit_rate = s.GetDouble();
	s = d["power"];
	power = s.GetDouble();

	vector<Position*> signal_info, meters, poles, daps;
	string ret;
	switch (action_id)
	{
	case KML:
	{
		signal_info = getPositionArrayFromJson(input, "signal_info");
		meters = getPositionArrayFromJson(input, "meters");
		poles = getPositionArrayFromJson(input, "poles");
		daps = getPositionArrayFromJson(input, "DAPs");
		KMLMethods* kml = new KMLMethods(meters, daps, poles, signal_info, scenario, technology, bit_rate, power, h_tx, h_rx, 1, mesh, rP);
		ret = kml->generateKML();
		delete kml;
		break;
	}
	case AUTOPLAN:
	{
		Value& s = d["overwrite"];
		bool overwrite = s.GetBool();
		if (overwrite)
			daps = getPositionArrayFromJson(input, "DAPs");
		meters = getPositionArrayFromJson(input, "meters");
		poles = getPositionArrayFromJson(input, "poles");
		int gridSize = 500;
		AutoPlanning* autoplan = new AutoPlanning(meters, poles, scenario, technology, bit_rate, power, h_tx, h_rx, 1, mesh, gridSize, rP);
		ret = autoplan->executeAutoPlan(redundancy, 0);
		delete autoplan;
		break;
	}
	case DRAW:
	{
		meters = getPositionArrayFromJson(input, "meters");
		daps = getPositionArrayFromJson(input, "DAPs");
		LinkCalculation* link = new LinkCalculation(meters, daps, scenario, technology, bit_rate, power, h_tx, h_rx, 1, mesh, rP);
		ret = link->executeLinkCalculation();
		delete link;
		break;
	}
	case METRIC:
	{
		meters = getPositionArrayFromJson(input, "meters");
		daps = getPositionArrayFromJson(input, "DAPs");
		MetricCalculation* metric = new MetricCalculation(meters, daps, scenario, technology, bit_rate, power, h_tx, h_rx, 1, mesh, rP);
		ret = metric->executeMetricCalculation();
		delete metric;
		break;
	}
	case GET_RANGE:
	{
		double dapdist = 0, step = 0.1, effs = 1;
		while (effs > MARGIN_VALUE)
		{
			dapdist += step;
			effs = getHataSRDSuccessRate(dapdist, scenario, technology, bit_rate, power, h_tx, h_rx, 1);
		}
		effs = 1;
		double meterdist = 0;
		while (effs > MARGIN_VALUE)
		{
			meterdist += step;
			effs = getHataSRDSuccessRate(meterdist, scenario, technology, bit_rate, power, h_tx, h_rx, 1);
		}
		ret = to_string(dapdist - step) + "\n" + to_string(meterdist - step);
		break;
	}
	default:
		break;
	}
	return ret;
}
int main(int argc, char** argv)
{
	string line, rP, input="";
	getline(cin, rP);
	rP += "/";
	while (getline(cin, line))
	{
		input += line+"\n";
	}
	////string wow = "0\n1\n2\n3\n5\n6\n20\n1\n2\n1\n-1\n18\n-22.889170233828658 -43.118748888373375\n-22.889127608916148 -43.118783086538315\n-22.888979348246664 -43.11858996748924\n-22.889104134320952 -43.11855107545853\n-22.889209152215333 -43.11853766441345\n-22.889320347544345 -43.11855509877205\n-22.889291930968938 -43.11875492334366\n-22.889235097800253 -43.118813931941986\n-22.889072011184073 -43.11885952949524\n-22.88900776488753 -43.118736147880554\n-22.88903741702818 -43.11853229999542\n-22.88917455809439 -43.11847999691963\n-22.889458723826547 -43.11822921037674\n-22.889311699022027 -43.118120580911636\n-22.889269691905763 -43.11819702386856\n-22.889086837248197 -43.117958307266235\n-22.889009000390597 -43.11814874410629\n-22.88906212714043 -43.11826676130329\n5\n-22.889067069162344 -43.118678480386734\n-22.889153554516668 -43.11850279569626\n-22.889246217335135 -43.1185644865036\n-22.88924745283894 -43.118730783462524\n-22.889154790021326 -43.11886489391327\n";
	//input = wow;
	//rP = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\";
	//input = "0\n1\n2\n3\n5\n6\n20\n1\n2\n1\n-1\n8\n-20.301143436776947 -40.28205871582031\n-20.3012516073493 -40.28202384710312\n-20.301357262254005 -40.28199166059494\n-20.3014276988171 -40.28217673301697\n-20.301246576161564 -40.282251834869385\n-20.301279278878937 -40.28225988149643\n-20.301161045945054 -40.28227597475052\n-20.301319528367745 -40.28220355510712\n3\n-20.30116859273076 -40.28216063976288\n-20.30129437243846 -40.282104313373566\n-20.30141763645291 -40.28206139802933\n";
	//rubyPath = rP;
	
	// 1. Parse a JSON string into DOM.
   // const char* json = input.c_str();

//	string rP = "/";
	
	//string input = "{\"scenario\":1,\"technology\":2,\"power\":20,\"mesh_hops\":2,\"rate\":6,\"h_tx\":3,\"h_rx\":5,\"redundancy\":1,\"action_id\":1,\"meters\":[{\"lat\":-17.52239827117121,\"lng\":-38.959472179412955},{\"lat\":-17.52234200060282,\"lng\":-38.95945876836777}],\"DAPs\":[{\"lat\":-17.52236118375305,\"lng\":-38.95949363708496},{\"lat\":-17.522372693642215,\"lng\":-38.95943060517311},{\"lat\":-17.52236118375305,\"lng\":-38.95946279168129},{\"lat\":-17.52235223161652,\"lng\":-38.95943731069565}]}";
	string ret = getResponse(input, rP);
	cout << ret;
}



//
//extern "C"
//
//void Init_Siris()
//{
//  Class rb_c = define_class("Siris")
//   .define_method("getResponse", &getResponse);
//
//}
//
