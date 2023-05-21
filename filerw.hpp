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
	string yl = "C";
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

string lmap()
{
	string td2 = "levels/map" + to_string(getlevel()) + ".map";
	return td2;
}

string bmap()
{
	string td1 = "levels/basemap" + to_string(getlevel()) + ".map";
	return td1;
}

string lnq()
{
	string q1 = "Level: " + to_string(getlevel());
	return q1;
}

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
	ifstream bMap(bmap());
	ofstream mapFile(lmap());
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
	fstream mapFile(lmap());
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
	ifstream mapFile(lmap());
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
	ifstream mapFile(lmap());
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
	int posb = getlevel()-1;
	int bl = (posb*56)+5;
	int bk = (getlevel()*56)+3;
	fstream mapFile("levels/stats.txt");
	if(mov<(findc(6,4)/f))
	{
		if(mov<10)
		{
			mapFile.seekp(bl, ios::beg);
			mapFile << "000" << to_string(mov);
		}
		else if(mov<100)
		{
			mapFile.seekp(bl, ios::beg);
			mapFile << "00" << to_string(mov);
		}
		else if(mov<1000)
		{
			mapFile.seekp(bl, ios::beg);
			mapFile << "0" << to_string(mov);
		}
		else if(mov<10000)
		{
			mapFile.seekp(bl, ios::beg);
			mapFile << to_string(mov);
		}
		else
		{
			mapFile.seekp(bl, ios::beg);
			mapFile << "9999";
		}
	}
	mapFile.seekp(bk, ios::beg);
	mapFile << "C";
//	lmap = "levels/map" + to_string(getlevel()) + ".map";
//	bmap = "levels/basemap" + to_string(getlevel()) + ".map";
}

int ftile(int ent, int entl)
{
	stringstream ft;
	string lvl;
	string xz;
	unsigned int wline = 0;
	ifstream mapFile(lmap());
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
	