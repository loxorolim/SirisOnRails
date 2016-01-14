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
		vector<string> opIds;
		for (SizeType j = 0; j < aux[i]["signal_info"].Size(); j++)
			opIds.push_back(aux[i]["signal_info"][j].GetString());

		Position* p = new Position(lat, lng, i,opIds);
		ret.push_back(p);
	}
	return ret;
}
string getResponse(string input, string rP)
{

	Document d;

	d.Parse(input.c_str());
	int action_id, scenario, technology, mesh = 0, redundancy = 1;
	double h_tx, h_rx, bit_rate, power, valid_cell_radius;
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
	s = d["valid_cell_radius"];
	valid_cell_radius = s.GetInt();

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
		KMLMethods* kml = new KMLMethods(meters, daps, poles, signal_info, scenario, technology, bit_rate, power, h_tx, h_rx, 1, mesh, valid_cell_radius, rP);
		ret = kml->generateKML();
		Document document;
		document.SetObject();
		Value v;
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		v.SetString(ret.c_str(), allocator);
		document.AddMember("KML", v, allocator);
		StringBuffer strbuf;
		Writer<StringBuffer> writer(strbuf);
		document.Accept(writer);
		delete kml;
		return strbuf.GetString();
		break;
	}
	case AUTOPLAN:
	{
		Value& s = d["overwrite"];
		bool overwrite = s.GetBool();

		meters = getPositionArrayFromJson(input, "meters");
		poles = getPositionArrayFromJson(input, "poles");
		int gridSize = 500;
		AutoPlanning* autoplan = new AutoPlanning(meters, poles, scenario, technology, bit_rate, power, h_tx, h_rx, 1, mesh, gridSize, rP);
		if (!overwrite)
		{
			daps = getPositionArrayFromJson(input, "DAPs");
			autoplan->setCoveredMeters(daps);
			//autoplan->removeAlreadyCoveredMeters(daps);
		}
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
			effs = getHataSRDSuccessRate(meterdist, scenario, technology, bit_rate, power, h_tx, h_tx, 1);
		}
		Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		Value v;
		v.SetDouble(dapdist - step);
		document.AddMember("DAPtoMeterDistance", v,allocator);
		v.SetDouble(meterdist - step);
		document.AddMember("MeterToMeterDistance", v, allocator);
		StringBuffer strbuf;
		Writer<StringBuffer> writer(strbuf);
		document.Accept(writer);
		ret = strbuf.GetString();
		break;
	}
	case PROCESS_KML:
	{		
					
		string received_kml = "";
		Value& s = d["KML"];
		
		received_kml = s.GetString();
		ret = processKML(received_kml);
		
		break;
	}
	default:
		break;
	}
	return ret;
}
int WebMain(int argc, char** argv)
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

	//input = "{\"scenario\":1,\"technology\":2,\"power\":20,\"mesh_hops\":2,\"rate\":6,\"h_tx\":3,\"h_rx\":5,\"redundancy\":1,\"action_id\":0,\"meters\":[{\"lat\":-22.956459,\"lng\":-43.06489099999999},{\"lat\":-22.956477,\"lng\":-43.06476199999997},{\"lat\":-22.956373,\"lng\":-43.06466499999999},{\"lat\":-22.956622,\"lng\":-43.064572},{\"lat\":-22.956758,\"lng\":-43.064513000000034},{\"lat\":-22.956832429095073,\"lng\":-43.06425780057907}],\"poles\":[{\"lat\":-22.956556,\"lng\":-43.064809999999966},{\"lat\":-22.956652,\"lng\":-43.06473299999999},{\"lat\":-22.956588,\"lng\":-43.064931},{\"lat\":-22.956403,\"lng\":-43.064926000000014},{\"lat\":-22.956988024850265,\"lng\":-43.06421756744385}],\"overwrite\":false,\"DAPs\":[{\"lat\":-22.95640040374783,\"lng\":-43.064831457672085}]}";
	//rubyPath = rP;
	
	// 1. Parse a JSON string into DOM.
   // const char* json = input.c_str();

//	string rP = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\";
	
//	string input = "{\"scenario\":1,\"technology\":2,\"power\":20,\"mesh_hops\":0,\"rate\":6,\"h_tx\":3,\"h_rx\":5,\"redundancy\":1,\"action_id\":0,\"meters\":[{\"lat\":-22.956459,\"lng\":-43.06489099999999},{\"lat\":-22.956477,\"lng\":-43.06476199999997},{\"lat\":-22.956373,\"lng\":-43.06466499999999},{\"lat\":-22.956622,\"lng\":-43.064572},{\"lat\":-22.956758,\"lng\":-43.064513000000034},{\"lat\":-22.956793,\"lng\":-43.064413},{\"lat\":-22.95681,\"lng\":-43.06429800000001},{\"lat\":-22.957045,\"lng\":-43.06424400000003},{\"lat\":-22.957089,\"lng\":-43.064099999999996},{\"lat\":-22.957126,\"lng\":-43.063941},{\"lat\":-22.956884,\"lng\":-43.064099999999996},{\"lat\":-22.956892,\"lng\":-43.06443999999999},{\"lat\":-22.957171,\"lng\":-43.06368599999996},{\"lat\":-22.957094,\"lng\":-43.06366800000001},{\"lat\":-22.956746,\"lng\":-43.06344999999999},{\"lat\":-22.956679,\"lng\":-43.063526000000024},{\"lat\":-22.956516,\"lng\":-43.063665000000015},{\"lat\":-22.956329,\"lng\":-43.063837000000035},{\"lat\":-22.95624,\"lng\":-43.064003000000014}],\"poles\":[{\"lat\":-22.956556,\"lng\":-43.064809999999966},{\"lat\":-22.956652,\"lng\":-43.06473299999999},{\"lat\":-22.956588,\"lng\":-43.064931},{\"lat\":-22.956403,\"lng\":-43.064926000000014},{\"lat\":-22.956882,\"lng\":-43.064526},{\"lat\":-22.956887,\"lng\":-43.064389000000006},{\"lat\":-22.956929,\"lng\":-43.064265999999975},{\"lat\":-22.957037,\"lng\":-43.064075},{\"lat\":-22.957092,\"lng\":-43.06387999999998}],\"overwrite\":false,\"DAPs\":[{\"lat\":-20.090068154178393,\"lng\":-42.94137239456177},{\"lat\":-22.956652,\"lng\":-43.06473299999999},{\"lat\":-22.957092,\"lng\":-43.06387999999998},{\"lat\":-22.956338,\"lng\":-43.06372399999998},{\"lat\":-22.955919,\"lng\":-43.064329999999984},{\"lat\":-22.956047,\"lng\":-43.06467900000001}]}";
	string ret = getResponse(input, rP);
	cout << ret;
	return 0;
}