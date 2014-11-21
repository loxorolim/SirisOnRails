#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include "auxiliars.h"
#include "MetricsResponse.h"
#include "HataSRD.h"

using namespace std;


vector<int> removeRepeated(vector<int> &v)
{
	vector<int> ret;
	for (int i = 0; i < v.size(); i++)
	{
		bool insert = true;
		for (int j = 0; j < ret.size(); j++)
		{
			if (v[i] == ret[j])
			{
				insert = false;
				break;
			}
				
		}
		if (insert)
			ret.push_back(v[i]);
	}
	return ret;

}
bool contains(vector<sComponent*> v, int elem)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i]->index == elem)
			return true;
	}
	return false;
}

double averageDirectLinkQuality(vector<vector<sComponent*>> statisticalList)
{
	double avgQuality = 0, numOfLinks = 0;
	for (int i = 0; i < statisticalList.size(); i++)
	{
		for (int j = 0; j < statisticalList[i].size(); j++)
		{
			if (statisticalList[i][j]->hop == 0)
			{
				avgQuality += statisticalList[i][j]->efficiency;
				numOfLinks++;
			}
		}

	}
	return avgQuality / statisticalList.size();

}
double calcEfficiency(sComponent *meter)
{
	double eff = 1;
	int hops = meter->hop;
	sComponent* m = meter;
	for (int i = 0; i < hops + 1; i++){
		eff *= m->efficiency;
		m = m->meshFather;
	}
	return eff;
};
vector<vector<double>> averageHops(vector<vector<sComponent*>> statisticalList, int meshMaxJumps)
{
	vector<vector<double>> avgHop;
	for (int i = 0; i < meshMaxJumps + 1; i++){
		double sum = 0;
		double qnt = 0;
		for (int j = 0; j < statisticalList.size(); j++)
		{
			for (int k = 0; k < statisticalList[j].size(); k++){
				if (statisticalList[j][k]->hop == i)
				{
					sum += calcEfficiency(statisticalList[j][k]);
					qnt++;
				}
			}
		}
		vector<double> toAdd;
		toAdd.push_back(sum / qnt);//AVG HOP EFFICIENCY
		toAdd.push_back(qnt); //HOP QNT
		avgHop.push_back(toAdd);
	}
	return avgHop;
}
vector<double> averageLinksPerDap(vector<vector<sComponent*>> statisticalList)
{
	double lpdSum = 0, max = -1, min = -1;
	for (int i = 0; i < statisticalList.size(); i++)
	{
		int nLinks = statisticalList[i].size();
		lpdSum += nLinks;
		if (max == -1 || nLinks > max)
			max = nLinks;
		if (min == -1 || nLinks < min)
			min = nLinks;
	}
	vector<double> alpdInfo;
	alpdInfo.push_back(lpdSum / statisticalList.size());//MEDIA
	alpdInfo.push_back(max);//MAX LINKS POR DAP
	alpdInfo.push_back(min);//MIN LINKS POR DAP

	return alpdInfo;
	//calcular desvio padrão
}
vector<sComponent*> noRepeat(vector<sComponent*> list)
{
	vector<sComponent*> ret;
	for (int i = 0; i < list.size(); i++)
	{
		bool add = true;
		for (int j = 0; j < ret.size(); j++)
		if (list[i]->index == ret[j]->index && add)
		{
			add = false;		
		}
		if (add)
			ret.push_back(list[i]);
	}
	return ret;

}
vector<vector<double>> avaregeMeshMetersPerDap(vector<vector<sComponent*>> statisticalList, int meshMaxJumps)
{
	vector<vector<double>> mmpdSum;
	for (int z = 0; z < meshMaxJumps + 1; z++)
	{
		double sum = 0, max = -1, min = -1;;
		for (int i = 0; i < statisticalList.size(); i++)
		{

			vector<sComponent*> uniqueArray = noRepeat(statisticalList[i]);
			int nMeters = 0;
			for (int j = 0; j< uniqueArray.size(); j++)
			if (uniqueArray[j]->hop == z)
				nMeters++;

			sum += nMeters;
			if (max == -1 || nMeters > max)
				max = nMeters;
			if (min == -1 || nMeters < min)
				min = nMeters;
		}
		vector<double> ammpdInfo;
		ammpdInfo.push_back(sum / statisticalList.size());
		ammpdInfo.push_back(max);
		ammpdInfo.push_back(min);
		ammpdInfo.push_back(z);
		//var ammpdInfo = {
		//avgMMPD: sum / statisticalList.length,
		//	 maxMMPD : max,
		//		   minMMPD : min,
		//				 hop : z
		//};
		mmpdSum.push_back(ammpdInfo);
	}

	return mmpdSum;
}
vector<double> averageMetersPerDap(vector<vector<sComponent*>> statisticalList)
{
	double mpdSum = 0, max = -1, min = -1;

	for (int i = 0; i < statisticalList.size(); i++){
		vector<sComponent*> uniqueArray = noRepeat(statisticalList[i]);
		int nMeters = uniqueArray.size();
		mpdSum += nMeters;
		if (max == -1 || nMeters > max)
			max = nMeters;
		if (min == -1 || nMeters < min)
			min = nMeters;
	}
	vector<double> ampdInfo;
	ampdInfo.push_back(mpdSum / statisticalList.size());
	ampdInfo.push_back(max);
	ampdInfo.push_back(min);
	//var ampdInfo = {
	//avgMPD: mpdSum / statisticalList.length,
	//	maxMPD : max,
	//		 minMPD : min
	//};
	return ampdInfo;
	//calcular desvio padrão
}
