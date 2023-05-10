#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include <sstream>

#include "cons.hpp"
#include "filerw.hpp"
#include "map.hpp"
#include "tank.hpp"

int tx(int jx)
{
	int ret = (f*(jx-1))/3;
	return ret;
}

int ty(int jy)
{
	int rey = f*(jy-1);
	return rey;
}

void sweffects()
{
	switch(getlevel())
	{
		case 1:
		if(ftile(3,22)==1)
		{
			frtile(tileset, '2', tx(22), ty(3));
		}
		else
		{
			frtile(tileset, '1', tx(22), ty(3));
		}
		break;
	}
}

void sweffects1()
{
	switch(getlevel())
	{
		case 1:
		if(ftile(11,22)==1)
		{
			for(int i=10; i<14; ++i)
			{
				frtile(tileset,'2',tx(13),ty(i));
			}
		}
		else
		{
			for(int i=10; i<14; ++i)
			{
				frtile(tileset,'1',tx(13),ty(i));
			}
		}
		break;
	}
}

void sweffects2()
{
	switch(getlevel())
	{
		case 1:
		cout << "nothin"
		break;
	}
}