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

class aTexture
{
	public:
		aTexture();
		~aTexture();
		bool loadFromFile(std::string path);
		bool loadFromRenderedText(std::string textureText,SDL_Color textColor);
		void free();
		void render(int x,int y,SDL_Rect* clip = NULL,int angle = 0,SDL_Point* center = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);
		int gW();
		int gH();
	private:
		SDL_Texture* mTexture;
		int mW;
		int mH;
};