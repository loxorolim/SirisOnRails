#include "KMLMethods.h"

string KMLMethods::generateKML()
{
	MetricCalculation* res = new MetricCalculation(meters, daps, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
	string metrics = res->executeMetricCalculation();
	delete res;

	LinkCalculation* links = new LinkCalculation(meters, daps, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
	vector<DrawInfo*> lines = links->calculateDrawingInfo();

	delete links;


	string init = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://earth.google.com/kml/2.2\">\n";
	init += "<Document>\n";
	init += "<Scenario>" + to_string(scenario) + "</Scenario>\n";
	init += "<Technology>" + to_string(technology) + "</Technology>\n";
	init += "<Power>" + to_string(t_power) + "</Power>\n";
	init += "<MeshHops>" + to_string(mesh) + "</MeshHops>\n";
	//init += "<Metrics>"+metrics+"</Metrics>\n";
	init += metrics + "\n";

	init += "<Folder><name>Medidores</name>\n";
		for (int i = 0; i<meters.size(); i++){
		init += "<Placemark>\n<name>Medidor</name>\n<Point>\n<coordinates>" +
			to_string(meters[i]->longitude)
			+ "," + to_string(meters[i]->latitude)
			+ ",0</coordinates>\n</Point>\n</Placemark>\n";
		}
	init += "</Folder>\n";
	init += "<Folder><name>Postes</name>\n";
	for (int i = 0; i < poles.size(); i++){
		init += "<Placemark>\n<name>Poste</name>\n<Point>\n<coordinates>" + to_string(poles[i]->longitude) + "," + to_string(poles[i]->latitude) + ",0</coordinates>\n</Point>\n</Placemark>\n";
	}
	init += "</Folder>\n";
	init += "<Folder><name>Agregadores</name>\n";
	for (int i = 0; i<daps.size(); i++){
		init += "<Placemark>\n<name>Agregador</name>\n<Point>\n<coordinates>" + to_string(daps[i]->longitude) + "," + to_string(daps[i]->latitude) + ",0</coordinates>\n</Point>\n</Placemark>\n";
	}
	init += "</Folder>\n";
	init += "<Folder><name>Enlaces</name>\n";
	for (int i = 0; i< lines.size(); i++){
		string alat, alng, blat, blng;
		alat = to_string(meters[lines[i]->a]->longitude);
		alng = to_string(meters[lines[i]->a]->latitude);
		if (lines[i]->hopnumber == 0)
		{
			blat = to_string(daps[lines[i]->b]->longitude);
			blng = to_string(daps[lines[i]->b]->latitude);
		}
		else
		{
			blat = to_string(meters[lines[i]->b]->longitude);
			blng = to_string(meters[lines[i]->b]->latitude);
		}

		init += "<Placemark>\n<name>Enlace</name>\n<efficiency>" + to_string(lines[i]->efficiency) + "</efficiency>\n<delay>" + to_string(lines[i]->delay) + "</delay>\n<LineString>\n<coordinates>\n" + alng + "," + alat + ",0\n" + blng + "," + blat + ",0\n</coordinates>\n</LineString>\n</Placemark>\n";
	}
	init += "</Folder>\n";
	init += "</Document>\n";
	init += "</kml>";
	return init;
}