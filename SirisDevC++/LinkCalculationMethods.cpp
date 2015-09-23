#include "LinkCalculationMethods.h"
//Esses métodos aqui servem pra retornar ao cliente dados de onde criar o enlace, suas qualiades etc.

//Ignore esse método, ele é o antigo.
vector<DrawInfo*> LinkCalculation::calculateDrawingInfoOld()
{
	vector<DrawInfo*> toCover;
	vector<Position*> coveredMeters;
	vector<Position*> allMarkers;

	//allMarkers.insert(allMarkers.end(), meters.begin(), meters.end());
	for(int i = 0; i < meters.size();i++)
	{
		allMarkers.push_back(meters[i]);
	}

	Grid* g = new Grid(allMarkers,daps, regionLimiter);
	g->putPositions(allMarkers);
	g->putPositions(daps);

	for (int d = 0; d < daps.size(); d++)
	{
		vector<Position*> markersReduced = g->getCell(daps[d]);
		for (int i = 0; i < markersReduced.size(); i++)
		{
			double dist = getDistance(daps[d], markersReduced[i]);
			double effs = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
			if (effs >= MARGIN_VALUE)
			{ //SE CONSIDERAR DAPS TEM Q ALTERA AKI PRA NAO CRIAR UMA LINHA COM ELE MESMO

				DrawInfo* toAdd = new DrawInfo(daps[d], markersReduced[i], effs, dist, 0);
				toCover.push_back(toAdd);
				coveredMeters.push_back(markersReduced[i]);
			}
		}
		//toCover = toCover.sort(function(a, b) { return a.value.distance - b.value.distance });
		//for (int i = 0; i < toCover.size(); i++)
		//	this.connect(toCover[i].marker, toCover[i].value);
	}
	if (meshEnabled)
	{
		sort(coveredMeters.begin(), coveredMeters.end());
		coveredMeters.erase(unique(coveredMeters.begin(), coveredMeters.end()), coveredMeters.end());
		vector<Position*> uncoveredMeters = removeVectorFromAnother(allMarkers, coveredMeters);
		vector<Position*> aux = coveredMeters;

		Grid* g2 = new Grid(aux,uncoveredMeters, regionLimiter);
		g2->putPositions(aux);
		for (int i = 0; i < meshEnabled; i++)
		{
			vector<Position*> newCovered;
			for (int j = 0; j < uncoveredMeters.size(); j++)
			{
				vector<Position*> toCheck = g2->getCell(uncoveredMeters[j]);
				DrawInfo* toAdd = chooseMeterToConnect(uncoveredMeters[j], aux);
				if (toAdd)
				{
					toCover.push_back(toAdd);
					newCovered.push_back(uncoveredMeters[j]);
					g2->putPosition(uncoveredMeters[j]);
				}
			}
			aux = newCovered;
			uncoveredMeters = removeVectorFromAnother(uncoveredMeters, newCovered);
		}
		delete g2;

	}
	delete g;
	return toCover;
}

vector<DrawInfo2*> LinkCalculation::calculateDrawingInfo()
{
	vector<DrawInfo2*> toCover;
	vector<Position*> connectedDevices;
	connectedDevices = daps;
	vector<Position*> uncoveredMeters = meters;
	vector<Position*> aux = connectedDevices;

	//Isso é feito para cada salto, considerando os saltos mesh.
	for (int i = 0; i < meshEnabled+1; i++)//esse +1 aqui é porque o meshEnabled diz a quantidade de saltos MESH, logo
	//se são 3 saltos mehs, temos 4 saltos ao todo!
	{
		//A ideia aqui é a seguinte:
		//Cada medidor vai tentar se conectar a um dispositivo já conectado. Os dispositivos já conectados inicialmente
		// são os DAPs. Para a próxima rodada, isto é, o segundo salto, considera-se que dispositivos conectados são
		//os que foram cobertos pelos DAP na primeira rodada(1 salto). Isso repete X vezes onde X é o número max de saltos.
		vector<Position*> newCovered;
		for (int j = 0; j < uncoveredMeters.size(); j++)
		{
			//vector<Position*> toCheck = g->getCell(uncoveredMeters[j]);
			DrawInfo2* toAdd = chooseDeviceToConnect(uncoveredMeters[j], aux, i);//Esse método que escolhe quem o medidor vai
			//se conectar. Ele sempre escolhe o cara mais próximo (isso é, com melhor qualidade). Então, no primeiro salto
			//ele só vai considerar os agregadores, a partir do segundo ele vai considerar também os medidores cobertos a
			//1 salto, depois a 2 e assim sucessivamente.
			if (toAdd)
			{
				toCover.push_back(toAdd);
				newCovered.push_back(uncoveredMeters[j]);
				//g->putPosition(uncoveredMeters[j]);
			}
		}
		aux = newCovered;
		uncoveredMeters = removeVectorFromAnother(uncoveredMeters, newCovered);
	}
	//delete g;
	return toCover;//retorna uma lista com dados DrawInfo (ve lá no auxiliars.h o que tem no DrawInfo)
	//PS: Seila pq o nome é toCover, devo ter copiado de alguma outra parte do código. hehe
}
//Calcula os enlaces depois os organiza num texto que é o cliente terá como resposta. Mais detalhes, ver o
//o toString do DrawInfo
string LinkCalculation::executeLinkCalculation()
{
	vector<DrawInfo2*> drawInfos = calculateDrawingInfo();
	string ret = "";
	for (int i = 0; i < drawInfos.size(); i++)
		ret += drawInfos[i]->toString() + " ";
	for (int i = 0; i < drawInfos.size(); i++)
		delete drawInfos[i];
	return ret;
}
//Ignora, isso era usado no método antigo.
DrawInfo* LinkCalculation::chooseMeterToConnect(Position* meter, vector<Position*> &connectedMeters)
{
	double minDist = -1;
	Position* meterToConnect = NULL;
	for (int i = 0; i < connectedMeters.size(); i++)
	{
		double dist = getDistance(meter, connectedMeters[i]);
		if (dist < minDist || minDist == -1)
		{
			minDist = dist;
			meterToConnect = connectedMeters[i];
		}
	}
	if (minDist != -1)
	{
		double dist = getDistance(meter, meterToConnect);
		double effs = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_TX, SRD);
		if (effs >= MARGIN_VALUE) {
			DrawInfo* ret = new DrawInfo(meter, meterToConnect, effs, dist, 1);
			return ret;
		}
	}
	return NULL;
}
//Aqui é o método que determina a quem o medidor vai se conectar e retorna um DrawInfo com as informações dessa
//conexão. Lembrando que ele se conecta sempre a um dispositivo já conectado com menor distância. De inicio, apenas
//os DAPs são considerados como dispositivos conectados, depois os medidores ligados a esse DAP também são considerados
//como conectados, depois os medidores que ligaram a outros medidores e assim por diante.
DrawInfo2* LinkCalculation::chooseDeviceToConnect(Position* meter, vector<Position*> &devices, int hopNumber)
{
	double minDist = -1;
	Position* deviceToConnect = NULL;
	for (int i = 0; i < devices.size(); i++)
	{
		double dist = getDistance(meter,devices[i]);
		if (dist < minDist || minDist == -1)
		{
			minDist = dist;
			deviceToConnect =devices[i];
		}
	}
	if (minDist != -1)
	{
		double dist = getDistance(meter, deviceToConnect);
		double effs = 0;
		if(hopNumber==0)
			effs = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
		else
			effs = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_TX, SRD);
	
		if (effs >= MARGIN_VALUE)
		{
			double linkDelay = calculateLinkDelay(effs, PCK_SIZE, BIT_RATE, technology) + hopNumber*PER_HOP_DELAY;
			DrawInfo2* ret;
			if(hopNumber == 0)
			{
				//ret = new DrawInfo(meter, deviceToConnect, effs, dist, 0);
				ret = new DrawInfo2(meter->index, deviceToConnect->index,hopNumber, effs, linkDelay, dist, 0);
			}
			else
			{
				//ret = new DrawInfo(meter, deviceToConnect, effs, dist, 1);
				ret = new DrawInfo2(meter->index, deviceToConnect->index, hopNumber, effs, linkDelay, dist, 1);
			}
			return ret;
		}
	}
	return NULL;
}
