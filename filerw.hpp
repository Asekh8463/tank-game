#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "cons.hpp"
#include "map.hpp"

using namespace std;

int getlevel()
{
	int aX = 1;
	string yl = to_string(isc);
	unsigned int wline = 0;
	string datas;
	ifstream mapFile("levels/stats.txt");
	while(getline(mapFile, datas))
	{
		wline++;
		if(datas.find(yl) != string::npos)
		{
			aX = wline;
		}
	}
	mapFile.close();
	return aX;
}

string lmap = "levels/map" + to_string(getlevel()) + ".map";
string bmap = "levels/basemap" + to_string(getlevel()) + ".map";

int findc(int p, int w)
{
	stringstream ft;
	string lvl;
	string xz;
	unsigned int wline = 0;
	int lnn = (64*(getlevel()-1))+getlevel();
	ifstream mapFile("levels/stats.txt");
	while(getline(mapFile, lvl))
	{
		wline++;
		if(wline == getlevel())
		{
			xz = lvl.substr((p-1), w);
		}
	}
	int fin1 = f*stoi(xz);
	mapFile.close();
	return fin1;
}

void reset()
{
	ifstream bMap(bmap);
	ofstream mapFile(lmap);
	string tc;
	while(getline(bMap, tc))
	{
		mapFile << tc << "\n";
	}
	bMap.close();
	mapFile.close();
}

void frtile(tile *tiles[], char r, int valx, int valy)
{
	int lne = ((valx/f)*3)+1;
	int lnp = (valy/f)+1;
	int pos;
	fstream mapFile(lmap);
	pos = (82*(lnp-1))+lne;
	mapFile.seekp(pos, ios::beg); //go to wherever
	mapFile << r;
	mapFile.close();
}

int readmapy(string usc)
{
	unsigned int wline = -1;
	int aX = -1;
	string datas;
	ifstream mapFile(lmap);
	while(getline(mapFile, datas))
	{
		wline++;
		if(datas.find(usc) != string::npos)
		{
			aX = (wline*f);
		}
	}
	mapFile.close();
	return aX;
}

int readmapx(string usc)
{
	int aY;
	string datas;
	ifstream mapFile(lmap);
	for(int i=1; i <= ((readmapy(usc)/f)+1); i++)
	{
		getline(mapFile, datas);
	}
	size_t loc = datas.find(usc);
	if(loc != string::npos)
	{
		aY = ((loc/3)*f);
	}
	else
	{
		aY = -1;
	}
	mapFile.close();
	return aY;
}

void clevel(int mov)
{
	int bl;
	int posb = getlevel()-1;
	int bk = (getlevel()*56)+4;
	fstream mapFile("stats.txt");
	if(mov<(findc(6,4)/f))
	{
		if(mov<10)
		{
			bl = (posb*56)+9;
			mapFile.seekg(posb, ios::beg);
			mapFile << mov;
		}
		else if(mov<100)
		{
			bl = (posb*56)+8;
			mapFile.seekg(posb, ios::beg);
			mapFile << mov;
		}
		else if(mov<1000)
		{
			bl = (posb*56)+7;
			mapFile.seekg(posb, ios::beg);
			mapFile << mov;
		}
		else if(mov<10000)
		{
			bl = (posb*56)+6;
			mapFile.seekg(posb, ios::beg);
			mapFile << mov;
		}
		else
		{
			bl = (posb*56)+6;
			mapFile.seekg(posb, ios::beg);
			mapFile << "9999";
		}
	}
	mapFile.seekg(bk, ios::beg);
	mapFile << isc;
}

int ftile(int ent, int entl)
{
	stringstream ft;
	string lvl;
	string xz;
	unsigned int wline = 0;
	ifstream mapFile(lmap);
	while(getline(mapFile, lvl))
	{
		wline++;
		if(wline == ent)
		{
			xz = lvl.substr((entl-1), 2);
		}
	}
	int fin = stoi(xz);
	mapFile.close();
	return fin;
}
	