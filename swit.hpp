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
		if((ftile(11,4)==0))
		{
			frtile(tileset,'2',tx(4),ty(11));
		}
		break;
	}
}

void unsweffects()
{
	switch(getlevel())
	{
		case 1:
		if((ftile(11,4)==2))
		{
			frtile(tileset,'0',tx(4),ty(11));
		}
		break;
	}
}

void sweffects1()
{
	switch(getlevel())
	{
		case 1:
		if(ftile(3,22)==0)
		{
			for(int i=3; i<5; ++i)
			{
				frtile(tileset,'2',tx(22),ty(i));
			}
		}
		else
		{
			for(int i=3; i<5; ++i)
			{
				frtile(tileset,'0',tx(22),ty(i));
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
		if(ftile(12,22)==0)
		{
			for(int i=12; i<15; ++i)
			{
				frtile(tileset,'2',tx(22),ty(i));
			}
		}
		else
		{
			for(int i=12; i<15; ++i)
			{
				frtile(tileset,'0',tx(22),ty(i));
			}
		}
		break;
	}
}