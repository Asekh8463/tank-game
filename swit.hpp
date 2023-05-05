#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <fstream>
#include <limits>

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
		cout << "nah";
		break;
	}
}

void sweffects1()
{
	switch(getlevel())
	{
		case 1:
		if(ftile(10,13)==6)
		{
			frtile(tileset,'2',tx(13),ty(10));
		}
		else
		{
			frtile(tileset,'6',tx(13),ty(10));
		}
		break;
	}
}

void sweffects2()
{
	switch(getlevel())
	{
		case 1:
		if(ftile(10,13)==6)
		{
			frtile(tileset,'2',tx(13),ty(10));
		}
		else
		{
			frtile(tileset,'6',tx(13),ty(10));
		}
		break;
	}
}