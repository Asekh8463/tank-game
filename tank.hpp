#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <fstream>

#include "cons.hpp"

class tank
{
	public:
	void stop();
	void rocketstop();
	void goback();
	void telrocket(SDL_Rect objct, std::string id);
	void movement(tile* tiles[],int lx,int ly);
	void keepmovin(tile* tiles[]);
	tank();
	void handleEvent(SDL_Event &e, tile* tiles[]);
	void move(tile* tiles[]);
	void render();
	void updn(tile* tiles[]);
	private:
	int mPosX, mPosY;
	int rPosX, rPosY;
};