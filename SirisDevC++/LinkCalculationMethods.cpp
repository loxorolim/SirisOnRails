#include "LinkCalculationMethods.h"
//Esses m�todos aqui servem pra retornar ao cliente dados de onde criar o enlace, suas qualiades etc.

vector<DrawInfo*> LinkCalculation::calculateDrawingInfo()
{
	vector<DrawInfo*> toCover;
	vector<Position*> connectedDevices;
	connectedDevices = daps;
	vector<Position*> uncoveredMeters = meters;
	vector<Position*> aux = connectedDevices;

	//Isso � feito para cada salto, considerando os saltos mesh.
	for (int i = 0; i < mesh+1; i++)//esse +1 aqui � porque o meshEnabled diz a quantidade de saltos MESH, logo
	//se s�o 3 saltos mehs, temos 4 saltos ao todo!
	{
		//A ideia aqui � a seguinte:
		//Cada medidor vai tentar se conectar a um dispositivo j� conectado. Os dispositivos j� conectados inicialmente
		// s�o os DAPs. Para a pr�xima rodada, isto �, o segundo salto, considera-se que dispositivos conectados s�o
		//os que foram cobertos pelos DAP na primeira rodada(1 salto). Isso repete X vezes onde X � o n�mero max de saltos.
		vector<Position*> newCovered;
		for (int j = 0; j < uncoveredMeters.size(); j++)
		{
			//vector<Position*> toCheck = g->getCell(uncoveredMeters[j]);
			DrawInfo* toAdd = chooseDeviceToConnect(uncoveredMeters[j], aux, i);//Esse m�todo que escolhe quem o medidor vai
			//se conectar. Ele sempre escolhe o cara mais pr�ximo (isso �, com melhor qualidade). Ent�o, no primeiro salto
			//ele s� vai considerar os agregadores, a partir do segundo ele vai considerar tamb�m os medidores cobertos a
			//1 salto, depois a 2 e assim sucessivamente.
			if (toAdd)
			{
				toCover.push_back(toAdd);
				newCovered.push_back(uncoveredMeters[j]);
				//g->putPosition(uncoveredMeters[j]);
			}
		}
		aux = newCovered;
		for (int j = 0; j < newCovered.size(); j++)
		{
			vector<Position*>::iterator it;
			it = find(uncoveredMeters.begin(), uncoveredMeters.end(), newCovered[j]);
			if (it != uncoveredMeters.end())
				uncoveredMeters.erase(it);			
		}

		//uncoveredMeters = removeVectorFromAnother(uncoveredMeters, newCovered);
	}
	//delete g;
	return toCover;//retorna uma lista com dados DrawInfo (ve l� no auxiliars.h o que tem no DrawInfo)
	//PS: Seila pq o nome � toCover, devo ter copiado de alguma outra parte do c�digo. hehe
}
//Calcula os enlaces depois os organiza num texto que � o cliente ter� como resposta. Mais detalhes, ver o
//o toString do DrawInfo
string LinkCalculation::executeLinkCalculation()
{
	vector<DrawInfo*> drawInfos = calculateDrawingInfo();

	Document document;
	document.SetObject();
	Value array(rapidjson::kArrayType);
	Document::AllocatorType& allocator = document.GetAllocator();

	for (int i = 0; i < drawInfos.size(); i++)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.SetObject();
		object.AddMember("a", drawInfos[i]->a, allocator);
		object.AddMember("b", drawInfos[i]->b, allocator);
		object.AddMember("hopnumber", drawInfos[i]->hopnumber, allocator);
		object.AddMember("efficiency", drawInfos[i]->efficiency, allocator);
		object.AddMember("delay", drawInfos[i]->delay, allocator);
		object.AddMember("distance", drawInfos[i]->distance, allocator);
		object.AddMember("dashed", drawInfos[i]->dashed, allocator);
		array.PushBack(object,allocator);
	}
	document.AddMember("DrawInfos", array, allocator);
	StringBuffer strbuf;
	Writer<StringBuffer> writer(strbuf);
	document.Accept(writer);
	return strbuf.GetString();

	string ret = "";
	for (int i = 0; i < drawInfos.size(); i++)
		ret += drawInfos[i]->toString() + " ";
	for (int i = 0; i < drawInfos.size(); i++)
		delete drawInfos[i];
	return ret;
}

//Aqui � o m�todo que determina a quem o medidor vai se conectar e retorna um DrawInfo com as informa��es dessa
//conex�o. Lembrando que ele se conecta sempre a um dispositivo j� conectado com menor dist�ncia. De inicio, apenas
//os DAPs s�o considerados como dispositivos conectados, depois os medidores ligados a esse DAP tamb�m s�o considerados
//como conectados, depois os medidores que ligaram a outros medidores e assim por diante.
DrawInfo* LinkCalculation::chooseDeviceToConnect(Position* meter, vector<Position*> &devices, int hopNumber)
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
			effs = getLinkQuality(dist);
		else
			effs = getLinkQualityBetweenMeters(dist);
	
		if (effs >= MARGIN_VALUE)
		{
			double linkDelay = calculateLinkDelay(effs, bit_rate, technology);//+hopNumber*PER_HOP_DELAY;
			DrawInfo* ret;
			if(hopNumber == 0)
				ret = new DrawInfo(meter->index, deviceToConnect->index,hopNumber, effs, linkDelay, dist, 0);
			else
				ret = new DrawInfo(meter->index, deviceToConnect->index, hopNumber, effs, linkDelay, dist, 1);
			return ret;
		}
	}
	return NULL;
}
