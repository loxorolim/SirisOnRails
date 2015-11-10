#include "KMLMethods.h"

string KMLMethods::getMetersKMLFormat()
{
	string init = "<Folder><name>Medidores</name>\n";
	for (int i = 0; i<meters.size(); i++)
	{
		init += "<Placemark>\n<name>Medidor</name>\n<Point>\n<coordinates>" +
			to_string(meters[i]->longitude)
			+ "," + to_string(meters[i]->latitude)
			+ ",0</coordinates>\n</Point>\n</Placemark>\n";
	}
	init += "</Folder>\n";
	return init;
}
string KMLMethods::getDAPsKMLFormat()
{
	string init = "<Folder><name>Agregadores</name>\n";
	for (int i = 0; i<daps.size(); i++){
		init += "<Placemark>\n<name>Agregador</name>\n<Point>\n<coordinates>" + to_string(daps[i]->longitude) + "," + to_string(daps[i]->latitude) + ",0</coordinates>\n</Point>\n</Placemark>\n";

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
	string init = "<Folder><name>Postes</name>\n";
	for (int i = 0; i < poles.size(); i++){
		init += "<Placemark>\n<name>Poste</name>\n<Point>\n<coordinates>" + to_string(poles[i]->longitude) + "," + to_string(poles[i]->latitude) + ",0</coordinates>\n</Point>\n</Placemark>\n";
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
	string init = "<Folder><name>Enlaces</name>\n";
	for (int i = 0; i< lines.size(); i++){
		string alat, alng, blat, blng;
		alat = to_string(meters[lines[i]->a]->longitude);
		alng = to_string(meters[lines[i]->a]->latitude);
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

		init += "<Placemark>\n<name>Enlace</name>\n<efficiency>" + to_string(lines[i]->efficiency) + "</efficiency>\n<delay>" + to_string(lines[i]->delay) + "</delay>\n<LineString>\n<coordinates>\n" + alng + "," + alat + ",0\n" + blng + "," + blat + ",0\n</coordinates>\n</LineString>\n</Placemark>\n";
	}

	init += "</Folder>\n";
	return init;
}
string KMLMethods::getHeatmapKMLFormat()
{
	string init = "</Folder>\n";
	init += "<Folder><name>Mapa de Calor</name>\n";
	for (int i = 0; i < coverageArea.size(); i++)
	{
		init += "<Placemark>\n<name>Ponto de coleta</name>\n<value>" + to_string(coverageArea[i]->weight) + "</value>\n";
		init += "<Operators>\n";
		for (int j = 0; j < coverageArea[i]->signalInfo.size(); j++)
		{
			init += "<value>" + to_string(coverageArea[i]->signalInfo[j]) + "</value>\n";
		}
		init += "</Operators>\n";
			
		init += "<Point>\n<coordinates>" + to_string(coverageArea[i]->longitude) + "," + to_string(coverageArea[i]->latitude) + ",0</coordinates>\n</Point>\n</Placemark>\n";
	}
	init += "</Folder>\n";
	return init;
}

string KMLMethods::generateKML()
{
	
	MetricCalculation* res = new MetricCalculation(meters, daps, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
	string metrics = res->executeMetricCalculation();
	//delete res;



	string init = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://earth.google.com/kml/2.2\">\n";
	init += "<Document>\n";
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
	init += "</Document>\n";
	init += "</kml>";
	
	return init;
}
void readKML(string inputFilename, vector<Position*>& daps, vector<Position*>& meters, vector<Position*>& poles )
{
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(inputFilename.c_str());
	pugi::xml_node	tools = doc.child("kml").child("Document").child("Folder");
	for (pugi::xml_node tool = tools; tool; tool = tool.next_sibling())
	{
		string type = tool.first_child().child_value();
		for (pugi::xml_node placemark = tool.child("Placemark"); placemark; placemark = placemark.next_sibling())
		{
			string coordinates = placemark.child("Point").child("coordinates").child_value();
		}
		
	}
	std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("kml").child("Document").child("Folder").child("name").child_value() << std::endl;
	std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("kml").child("Document").child("Folder").child("name").child_value() << std::endl;
}