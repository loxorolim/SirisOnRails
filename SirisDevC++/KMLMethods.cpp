#include "KMLMethods.h"

string KMLMethods::getMetersKMLFormat()
{
	vector<Position*> m, d;
	for (int i = 0; i < meters.size(); i++)
	{
		Position* p = new Position(meters[i]->latitude, meters[i]->longitude, i);
		m.push_back(p);
	}
	for (int i = 0; i <daps.size(); i++)
	{
		Position* p = new Position(daps[i]->latitude, daps[i]->longitude, i);
		d.push_back(p);
	}
	MetricCalculation* mc = new MetricCalculation(m, d, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
	vector<sComponent*> sComponents = mc->statisticalList();

	string init = "<Folder><name>"+MetersTag+"</name>\n";
	for (int i = 0; i<meters.size(); i++)
	{
		double meterSDR = 1, meterDelay = 0;
		int hop = -1;
		if (i < sComponents.size()) // OU SEJA, SE O MEDIDOR ESTIVER COBERTO
		{
			sComponent* aux = sComponents[i];
			meterDelay += aux->hop*PER_HOP_DELAY;
			hop = aux->hop;
			while (aux != NULL)
			{
				meterSDR *= aux->efficiency;
				meterDelay += aux->delay;
				aux = aux->meshFather;
			}
		}
		init += "<Placemark>\n<name>" + meter_name + "</name>";
		if (i < sComponents.size())
		{
			init += "\n<efficiency>" + to_string(meterSDR) + "</efficiency>\n<delay>" + to_string(meterDelay) + "</delay>\n<meshHop>" + to_string(hop) + "</meshHop>";
		}
		init += "\n<Point>\n<coordinates>"+
			to_string(meters[i]->longitude)
			+ "," + to_string(meters[i]->latitude)
			+ ",0</coordinates>\n</Point>\n</Placemark>\n";
	}
	init += "</Folder>\n";
	return init;
}
string KMLMethods::getDAPsKMLFormat()
{
	string init = "<Folder><name>"+DAPsTag +"</name>\n";
	for (int i = 0; i<daps.size(); i++){
		init += "<Placemark>\n<name>"+DAP_name +"</name>\n<Point>\n<coordinates>" + to_string(daps[i]->longitude) + "," + to_string(daps[i]->latitude) + ",0</coordinates>\n</Point>\n</Placemark>\n";

	}
	init += "</Folder>\n";
	return init;
}
string KMLMethods::getMetricKMLFormat()
{
	vector<Position*> m, d;
	for (int i = 0; i < meters.size(); i++)
	{
		Position* p = new Position(meters[i]->latitude, meters[i]->longitude, i);
		m.push_back(p);
	}
	for (int i = 0; i <daps.size(); i++)
	{
		Position* p = new Position(daps[i]->latitude, daps[i]->longitude, i);
		d.push_back(p);
	}
	MetricCalculation* res = new MetricCalculation(m, d, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
	string metrics = res->executeMetricCalculation();
	delete res;
	return metrics;
}
string KMLMethods::getPolesKMLFormat()
{
	string init = "<Folder><name>"+ PolesTag +"</name>\n";
	for (int i = 0; i < poles.size(); i++){
		init += "<Placemark>\n<name>"+pole_name +"</name>\n<Point>\n<coordinates>" + to_string(poles[i]->longitude) + "," + to_string(poles[i]->latitude) + ",0</coordinates>\n</Point>\n</Placemark>\n";
	}
	init += "</Folder>\n";
	return init;
}
string KMLMethods::getLinksKMLFormat()
{
	vector<Position*> m, d;
	for (int i = 0; i < meters.size(); i++)
	{
		Position* p = new Position(meters[i]->latitude, meters[i]->longitude, i);
		m.push_back(p);
	}
	for (int i = 0; i <daps.size(); i++)
	{
		Position* p = new Position(daps[i]->latitude, daps[i]->longitude, i);
		d.push_back(p);
	}
	LinkCalculation* links = new LinkCalculation(m, d, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
	vector<DrawInfo*> lines = links->calculateDrawingInfo();
	delete links;
	string init = "<Folder><name>"+ LinksTag +"</name>\n";
	for (int i = 0; i< lines.size(); i++){
		string alat, alng, blat, blng;
		alat = to_string(meters[lines[i]->a]->latitude);
		alng = to_string(meters[lines[i]->a]->longitude);
		if (lines[i]->hopnumber == 0)
		{
			blng = to_string(daps[lines[i]->b]->longitude);
			blat = to_string(daps[lines[i]->b]->latitude);
		}
		else
		{
			blng = to_string(meters[lines[i]->b]->longitude);
			blat = to_string(meters[lines[i]->b]->latitude);
		}

		init += "<Placemark>\n<name>"+link_name +"</name>\n<efficiency>" + to_string(lines[i]->efficiency) + "</efficiency>\n<delay>" + to_string(lines[i]->delay) + "</delay>\n<LineString>\n<coordinates>\n" + alng + "," + alat + ",0\n" + blng + "," + blat + ",0\n</coordinates>\n</LineString>\n</Placemark>\n";
	}

	init += "</Folder>\n";
	return init;
}
string KMLMethods::getHeatmapKMLFormat()
{
	string init = "";
	init += "<Folder><name>"+SignalsTag+"</name>\n";
	for (int i = 0; i < coverageArea.size(); i++)
	{
		init += "<Placemark>\n<name>" + signal_name + "</name>\n";//<value>" + to_string(coverageArea[i]->weight) + "</value>\n";
		init += "<Operators>\n";
		for (int j = 0; j < coverageArea[i]->signalInfo.size(); j++)
		{
			init += "<id>" + coverageArea[i]->signalInfo[j] + "</id>\n";
		}
		init += "</Operators>\n";
			
		init += "<Point>\n<coordinates>" + to_string(coverageArea[i]->longitude) + "," + to_string(coverageArea[i]->latitude) + ",0</coordinates>\n</Point>\n</Placemark>\n";
	}
	init += "</Folder>\n";
	return init;
}

string KMLMethods::generateKML()
{
	if(verbose)
	{
		 cout << "\n Gerando arquivo KML";
		 cout << "\n Calculando estatisticas";
	}
	MetricCalculation* res = new MetricCalculation(meters, daps, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
	string metrics = res->executeMetricCalculation();
	if (verbose) cout << "\n Estatisticas calculadas";
	//delete res;


	if (verbose) cout << "\n Escrevendo elementos do KML";
	string init = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://earth.google.com/kml/2.2\">\n";
	init += "<Scenario>" + to_string(scenario) + "</Scenario>\n";
	init += "<Technology>" + to_string(technology) + "</Technology>\n";
	init += "<Power>" + to_string(t_power) + "</Power>\n";
	init += "<MeshHops>" + to_string(mesh) + "</MeshHops>\n";
	//init += "<Metrics>"+metrics+"</Metrics>\n";
	init += metrics + "\n";
	init += getMetersKMLFormat();
	init += getPolesKMLFormat();
	init += getDAPsKMLFormat();
	init += getLinksKMLFormat();
	init += getHeatmapKMLFormat();
	init += "</kml>";
	if (verbose) cout << "\n KML gerado";
	return init;
}
void KMLMethods::saveKmlToFile(string filename)
{
	ofstream f(filename.c_str());
	string resp = generateKML();
	f << resp;
	f.close();
	if (verbose) cout << "\n Arquivo KML salvo em: " + filename;
}
int readKML(string inputFilename, vector<Position*>& daps, vector<Position*>& meters, vector<Position*>& poles, vector<Position*>& coverageArea )
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(inputFilename.c_str());
	if (result.status)
	{
		cerr << "\nErro ao ler o arquivo. Verifique se o diretorio e o nome estao corretos.";
		return 1;
	}
	pugi::xml_node	tools = doc.child("kml").child("Folder");
	for (pugi::xml_node tool = tools; tool; tool = tool.next_sibling())
	{
		string type = tool.first_child().child_value();
		if (type == MetersTag || type == DAPsTag || type == PolesTag || type == SignalsTag)
		{
			for (pugi::xml_node placemark = tool.child("Placemark"); placemark; placemark = placemark.next_sibling())
			{
				string coordinates = placemark.child("Point").child("coordinates").child_value();
				vector<string> coordinatesVector = split(coordinates, ',');
				double lat = stof(coordinatesVector[1]), lng = stof(coordinatesVector[0]);
				if (type == MetersTag)
				{
					Position* p = new Position(lat, lng, meters.size());
					meters.push_back(p);
				}
				if (type == DAPsTag)
				{
					Position* p = new Position(lat, lng, daps.size());
					daps.push_back(p);
				}
				if (type == PolesTag)
				{
					Position* p = new Position(lat, lng, poles.size());
					poles.push_back(p);
				}
				if (type == SignalsTag)
				{
					vector<string> ids;
					for (pugi::xml_node i = placemark.child("Operators").child("id"); i; i = i.next_sibling())
					{
						string signal_op = i.child_value();
						ids.push_back(signal_op);
					}
					Position* p = new Position(lat, lng, coverageArea.size(), ids);
					coverageArea.push_back(p);
				}
			}
		}		
	}
	return 0;
}
void convertMeterAndPolesToKml(string metersFilePath, string polesFilePath, string toSave)
{
	FILE * file;
	file = fopen(metersFilePath.c_str(), "r");
	FILE * file2;
	file2 = fopen(polesFilePath.c_str(), "r");


	vector<Position*> meters;
	vector<Position*> poles;

	while (true)
	{
		double lat = -1;
		double lng = -1;
		fscanf(file, "%lf %lf", &lng, &lat);
		if (lat == -1)
			break;
		Position *toAdd = new Position(lat, lng, meters.size());
		meters.push_back(toAdd);
	}
	while (true)
	{
		double lat = -1;
		double lng = -1;
		fscanf(file2, "%lf %lf", &lng, &lat);
		if (lat == -1)
			break;
		Position *toAdd = new Position(lat, lng, poles.size());
		poles.push_back(toAdd);
	}

	fclose(file);
	fclose(file2);

	vector<Position*> nullVec;
	KMLMethods* kmethods = new KMLMethods(meters, nullVec, poles, nullVec, 0, 0, 0, 0, 0, 0, 0, 0, "");
	kmethods->saveKmlToFile(toSave);
	delete kmethods;
}