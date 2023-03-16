#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <fstream>

class tile
{
	public:
	tile(int x,int y,int type);
	void render();
	SDL_Rect gBlock();
	int gType();
	private:
	SDL_Rect block;
	int tType;
};