//TODO:
//rocket explosion rect
//Reset rocket location after explosion
//show rocketfired indicator
//mines!
//teleport DO NEXT
//moving hazards
/*later*/
//level selection

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <fstream>

#include "map.hpp"
#include "texture.hpp"
#include "tank.hpp"

using namespace std;

const int f = 40;
const int noTiles = 378;
const int startx = 26*f;
const int starty = f;

double pointing = 0.0;
double pointing1 = 0.0;

bool rocketfired;

SDL_Rect sprite1;
SDL_Rect rect1a;
SDL_Rect rect1b;
SDL_Rect rect1c;
SDL_Rect rect1d;
SDL_Rect rocketrect;

SDL_Rect rect2 = {16*f,0,f,14*f}; //wall 1
SDL_Rect rect3 = {25*f,5*f,f,f}; //test for teleport

//tile types
const int BREAKABLE = 0;
const int UNBREAKABLE = 1;
const int BLANK = 2;

tile* tileset[noTiles];

bool init();
bool loadMedia(tile *tiles[]);
void exit(tile *tiles[]);

bool checktiles(SDL_Rect A, SDL_Rect B);

bool setTiles(tile *tiles[]);
bool iswall(SDL_Rect input1, tile* tiles[]);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font *font = NULL;

SDL_Rect spriteClips[2];
aTexture TANK;
aTexture TEXTure;
aTexture BLOCKS;
aTexture ROCKET;
SDL_Rect tileclips[3];

void tank::stop()
{
	mPosY = mPosY;
	mPosX = mPosX;
	pointing = pointing;
	rPosX = rPosX;
	rPosY = rPosY;
	rocketrect.x = rPosX;
	rocketrect.y = rPosY;
	sprite1.x = mPosX;
	sprite1.y=mPosY;
}

void tank::rocketstop()
{
	rPosX=mPosX;
	rPosY=mPosY;
	rocketrect.x=mPosX;
	rocketrect.y=mPosY;
	rocketfired=false;
	sprite1.x=mPosX;
	sprite1.y=mPosY;
}

void tank::goback()
{
	mPosX=startx;
	mPosY=starty;
	sprite1.x=startx;
	sprite1.y=starty;
	rect1a.x = startx-1;
	rect1a.y = starty;
	rect1b.x = startx+f;
	rect1b.y = starty;
	rect1c.x = startx;
	rect1c.y = starty-1;
	rect1d.x = startx;
	rect1d.y = starty+f;
	if(!rocketfired)
	{
		rocketstop();
	}
}

aTexture::aTexture()
{
	mTexture = NULL;
	mW = 0;
	mH = 0;
}

aTexture::~aTexture()
{
	free();
}

bool aTexture::loadFromFile(std::string path)
{
	free();
	SDL_Texture* nTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL)
	{
		cout << "Unable to load image(s)! SDL_image Error: %s\n" << path.c_str() << IMG_GetError() << "||";
	}
	SDL_SetColorKey(loadedSurface,SDL_TRUE,SDL_MapRGB(loadedSurface->format,0,0xFF,0xFF));
	nTexture = SDL_CreateTextureFromSurface(renderer,loadedSurface);
	mW = loadedSurface->w;
	mH = loadedSurface->h;
	SDL_FreeSurface(loadedSurface);
	mTexture = nTexture;
	return mTexture != NULL;
}

bool aTexture::loadFromRenderedText(std::string textureText,SDL_Color textColor)
{
	free();
	SDL_Surface* textSurface = TTF_RenderText_Solid(font,textureText.c_str(),textColor);
	mTexture = SDL_CreateTextureFromSurface(renderer,textSurface);
	mW = textSurface->w;
	mH = textSurface->h;
	SDL_FreeSurface(textSurface);
	return mTexture != NULL;
}

void aTexture::free()
{
	SDL_DestroyTexture(mTexture);
	mTexture = NULL;
	mW = 0;
	mH = 0;
}

void aTexture::render(int x,int y,SDL_Rect* clip,double angle,SDL_Point* center,SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = {x,y,mW,mH};
	if(clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(renderer,mTexture,clip,&renderQuad,angle,center,flip);
}

int aTexture::gW()
{
	return mW;
}

int aTexture::gH()
{
	return mH;
}

tile::tile(int x,int y,int type)
{
	block.x = x;
	block.y = y;
	block.w = f;
	block.h = f;
	tType = type;
}

void tile::render()
{
	BLOCKS.render(block.x,block.y,&tileclips[tType]);
}

SDL_Rect tile::gBlock()
{
	return block;
}

int tile::gType()
{
	return tType;
}

tank::tank()
{
	//tank itself
	mPosX = startx; 
	mPosY = starty;
	//left collision
	rect1a.x = startx-1;
	rect1a.y = starty;
	rect1a.w = 1;
	rect1a.h = f;
	//right collision
	rect1b.x = startx+f;
	rect1b.y = starty;
	rect1b.w = 1;
	rect1b.h = f;
	//top collision
	rect1c.x = startx;
	rect1c.y = starty-1;
	rect1c.w = f;
	rect1c.h = 1;
	//bottom collision
	rect1d.x = startx;
	rect1d.y = starty+f;
	rect1d.w = f;
	rect1d.h = 1;
	rPosX = startx;
	rPosY = starty;
	//setting rects start pos
	rocketrect.x = startx;
	rocketrect.y = starty;
	rocketrect.w = f;
	rocketrect.h = f;
	sprite1.x=startx;
	sprite1.y=starty;
	sprite1.w=f;
	sprite1.h=f;
}

void tank::keepmovin(tile* tiles[])
{
	//SDL_bool colliding7 = SDL_HasIntersection(&rocketrect, &rect3); disabling teleporter for now
	if(rocketfired==true)
	{
		if(pointing1==0) //down
		{
			rPosY=rPosY+f;
			rocketrect.y=rocketrect.y+f;
			if(iswall(rocketrect,tiles))
			{
				rocketstop();
			}
			else
			{
				rPosY=rPosY+f;
			}
/*			if(colliding7)
			{
				rPosX=40;
				rPosY=40;
				rocketrect.x = 40;
				rocketrect.y = 40;
			}*/
		}
		if(pointing1==-90) //right
		{
			rPosX=rPosX+f;
			rocketrect.x=rocketrect.x+f;
			if(iswall(rocketrect,tiles))
			{
				rocketstop();
			}
			else
			{
				rPosX=rPosX+f;
			}
/*			if(colliding7)
			{
				rPosX=40;
				rPosY=40;
				rocketrect.x = 40;
				rocketrect.y = 40;
			}*/
		}
		if(pointing1==180)//up
		{
			rPosY=rPosY-f;
			rocketrect.y=rocketrect.y-f;
			if(iswall(rocketrect,tiles))
			{
				rocketstop();
			}
			else
			{
				rPosY=rPosY-f;
			}
/*			if(colliding7)
			{
				rPosX=40;
				rPosY=40;
				rocketrect.x = 40;
				rocketrect.y = 40;
			}*/
		}
		if(pointing1==90) //left
		{
			rPosX=rPosX-f;
			rocketrect.x=rocketrect.x-f;
			if(iswall(rocketrect,tiles))
			{
				rocketstop();
			}
			else
			{
				rPosX=rPosX-f;
			}
/*			if(colliding7)
			{
				rPosX=40;
				rPosY=40;
				rocketrect.x = 40;
				rocketrect.y = 40;
			}*/
		}
	}
	else
	{
		rPosX=mPosX;
		rPosY=mPosY;
		rocketrect.x = rPosX;
		rocketrect.y = rPosY;
	}
}

void tank::handleEvent(SDL_Event &e, tile* tiles[])
{
	SDL_bool colliding1 = SDL_HasIntersection(&rect1a,&rect2);
	SDL_bool colliding2 = SDL_HasIntersection(&rect1b,&rect2);
	SDL_bool colliding3 = SDL_HasIntersection(&rect1c,&rect2);
	SDL_bool colliding4 = SDL_HasIntersection(&rect1d,&rect2);
//	SDL_bool colliding6 = SDL_HasIntersection(&sprite1,&rect3); //disable teleports for now
//	SDL_bool colliding7 = SDL_HasIntersection(&rocketrect, &rect3);
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_UP:
			if(iswall(rect1c,tiles))
			{
				mPosY = mPosY;
				rect1a.y = mPosY;
				rect1b.y = mPosY;
				rect1c.y = mPosY-1;
				sprite1.y = mPosY;
			}
			else
			{
				mPosY = mPosY-f;
				rect1a.y = rect1a.y-f;
				rect1b.y = rect1b.y-f;
				rect1c.y = rect1c.y-f;
				rect1d.y = rect1d.y-f;
				sprite1.y = mPosY-f;
				pointing = 180;
			}
			keepmovin(tileset);
/*			if(colliding6)
			{
				goback();
			}*/
			break;
			case SDLK_DOWN:
			if(iswall(rect1d,tiles))
			{
				mPosY = mPosY;
				rect1a.y = mPosY;
				rect1b.y = mPosY;
				rect1c.y = mPosY-1;
				rect1d.y = mPosY+f;
				sprite1.y = mPosY;
			}
			else
			{
				mPosY = mPosY+f;
				rect1a.y = rect1a.y+f;
				rect1b.y = rect1b.y+f;
				rect1c.y = rect1c.y+f;
				rect1d.y = rect1d.y+f;
				sprite1.y = mPosY+f;
				pointing = 0;
			}
			keepmovin(tileset);
/*			if(colliding6)
			{
				goback();
			}*/
			break;
			case SDLK_LEFT:
			if(iswall(rect1a,tiles))
			{
				mPosX = mPosX;
				rect1a.x = mPosX-1;
				rect1b.x = mPosX+f;
				rect1c.x = mPosX;
				sprite1.x = sprite1.x;
			}
			else
			{
				mPosX = mPosX-f;
				rect1a.x = rect1a.x-f;
				rect1b.x = rect1b.x-f;
				rect1c.x = rect1c.x-f;
				rect1d.x = rect1d.x-f;
				sprite1.x = sprite1.x-f;
				pointing = 90;
			}
			keepmovin(tileset);
/*			if(colliding6)
			{
				goback();
			}*/
			break;
			case SDLK_RIGHT:
			if(iswall(rect1b,tiles))
			{
				mPosX = mPosX;
				rect1a.x = mPosX-1;
				rect1b.x = mPosX+f;
				rect1c.x = mPosX;
				sprite1.x = sprite1.x;
			}
			else
			{
				mPosX = mPosX+f;
				rect1a.x = rect1a.x+f;
				rect1b.x = rect1b.x+f;
				rect1c.x = rect1c.x+f;
				rect1d.x = rect1d.x+f;
				sprite1.x = sprite1.x+f;
				pointing = -90;
			}
			keepmovin(tileset);
/*			if(colliding6)
			{
				goback();
			}*/
			break;
			case SDLK_SPACE:
			if(!rocketfired)
			{
				rocketfired = true;
				if(pointing == 0) //facing down
				{
					pointing1 = 0;
					rPosY = rPosY+f;
					rocketrect.y=rocketrect.y+f;
					if(iswall(rocketrect,tiles))
					{
						rocketstop();
					}
					else
					{
						rPosY = rPosY+f; //keep moving
					}
				}
				else if(pointing == 90) //facing right
				{
					pointing1 = 90;
					rPosX = rPosX-f;
					rocketrect.x=rocketrect.x-f;
					if(iswall(rocketrect,tiles))
					{
						rocketstop();
					}
					else
					{
						rPosX = rPosX-f; //keep moving
					}
				}
				else if(pointing == 180)//facing up
				{
					pointing1 = 180;
					rPosY = rPosY-f;
					rocketrect.y=rocketrect.y-f;
					if(iswall(rocketrect,tiles))
					{
						rocketstop();
					}
					else
					{
						rPosY = rPosY-f; //keep moving
					}
				}
				else //facing left
				{
					pointing1 = -90;
					rPosX = rPosX+f;
					rocketrect.x=rocketrect.x+f;
					if(iswall(rocketrect,tiles))
					{
						rocketstop();
					}
					else
					{
						rPosX = rPosX+f; //keep moving
					}
				}
/*				if(colliding7)
				{
					rPosX=40;
					rPosY=40;
					rocketrect.x = 40;
					rocketrect.y = 40;
				}*/
				break;
			}
		}
	}
}

void tank::move(tile* tiles[])
{
	
	if(mPosX<0) //too far left
	{
		mPosX = 0;
		rect1a.x = -1;
		rect1a.y = mPosY;
		rect1b.x = f;
		rect1b.y = mPosY;
		rect1c.x = 0;
		rect1d.x = 0;
		rPosX = rPosX;
		sprite1.x = sprite1.x;
		sprite1.y = sprite1.y;
	}
	if(mPosY<0) //too far up
	{
		mPosY = 0;
		rect1a.x = mPosX-1;
		rect1a.y = mPosY;
		rect1b.x = mPosX+f;
		rect1b.y = mPosY;
		rect1c.y = mPosY-1;
		rect1d.y = mPosY+f;
		rPosY = rPosY;
		sprite1.x = sprite1.x;
		sprite1.y = sprite1.y;
	}
	if(mPosX>1040) // too far right
	{
		mPosX = 1040;
		rect1a.x = 1039;
		rect1a.y = mPosY;
		rect1b.x = 27*f;
		rect1b.y = mPosY;
		rect1c.x = 26*f;
		rect1d.x = 26*f;
		rPosX = rPosX;
		sprite1.x = sprite1.x;
		sprite1.y = sprite1.y;
	}
	if(mPosY>520) //too far down
	{
		mPosY = 520;
		rect1a.x = mPosX-1;
		rect1a.y = mPosY;
		rect1b.x = mPosX+f;
		rect1b.y = mPosY;
		rect1c.y = mPosY-1;
		rect1d.y = mPosY+f;
		rPosY = rPosY;
		sprite1.x = sprite1.x;
		sprite1.y = sprite1.y;
	}
	if((rPosX<0)||(rPosY<0)||(rPosY>520)||(rPosX>1040))
	{
		rPosX=mPosX;
		rocketrect.x=mPosX;
		rPosY=mPosY;
		rocketrect.y=rPosY;
		rocketfired=false;
	}
	if(SDLK_UP && SDLK_RIGHT)
	{
	stop();
	}
	if(SDLK_UP && SDLK_LEFT)
	{
	stop();
	}
	if(SDLK_DOWN && SDLK_RIGHT)
	{
	stop();
	}
	if(SDLK_DOWN && SDLK_LEFT)
	{
	stop();
	}
	if(iswall(rocketrect,tiles))
	{
	rocketstop();
	}
}

void tank::render()
{
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	ROCKET.render(rPosX,rPosY,&spriteClips[1],pointing1,NULL,flipType);
	TANK.render(mPosX,mPosY,&spriteClips[0],pointing,NULL,flipType);
}

bool init()
{
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL couldn't initialize! SDL_Error: " << SDL_GetError() << "||";
		success = false;
	}
	window = SDL_CreateWindow("random tank game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 27*f, 14*f, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	int imgFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags)&imgFlags))
	{
		cout << "SDL_image couldn't initialize! SDL_image Error: " << IMG_GetError() << "||";
	}
	if(TTF_Init() == -1)
	{
		cout << "SDL_ttf couldn't initialize! SDL_ttf Error:" << TTF_GetError() << "||";
		success = false;
	}
	return success;
}

bool loadMedia(tile* tiles[])
{
	bool success = true;

	font = TTF_OpenFont("font.ttf",20);
	SDL_Color textColor = {0,0,0};

	if(!TEXTure.loadFromRenderedText("Level: 1",textColor))
	{
		cout << "Failed to render text!" << TTF_GetError() << "||";
		success = false;
	}

	if(!BLOCKS.loadFromFile("grasses.png"))
	{
		cout << "Failed to load tile set!||";
		success = false;
	}
	if(!setTiles(tiles))
	{
		cout << "Failed to load tile set!||";
		success = false;
	}

	if(!TANK.loadFromFile("grasses.png"))
	{
		cout << "Failed to load spritesheet!||";
		success = false;
	}
	else
	{
		spriteClips[0].x = f;
		spriteClips[0].y = 0;
		spriteClips[0].w = f;
		spriteClips[0].h = f;
	}
	if(!ROCKET.loadFromFile("grasses.png"))
	{
		cout << "Failed to load spritesheet!||";
		success = false;
	}
	else
	{
		spriteClips[1].x = 0;
		spriteClips[1].y = f;
		spriteClips[1].w = f;
		spriteClips[1].h = f;
	}

	return success;
}

void exit(tile *tiles[])
{
	for(int i = 0;i < noTiles;++i)
	{
		if(tiles[i] != NULL)
		{
			delete tiles[i];
			tiles[i] = NULL;
		}
	}
	TANK.free();
	TEXTure.free();
	ROCKET.free();
	BLOCKS.free();
	TTF_CloseFont(font);
	font = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

bool checktiles(SDL_Rect A, SDL_Rect B)
{
	int La,Lb;
	int Ra, Rb;
	int Ta, Tb;
	int Ba, Bb;
	
	La = A.x;
	Ra = A.x+A.w;
	Ta = A.y;
	Ba = A.y+A.h;

	Lb = B.x;
	Rb = B.x+B.w;
	Tb = B.y;
	Bb = B.y+B.h;
	
    if( Ba <= Tb )
    {
        return false;
    }

    if( Ta >= Bb )
    {
        return false;
    }

    if( Ra <= Lb )
    {
        return false;
    }

    if( La >= Rb )
    {
        return false;
    }
	return true;
}

bool setTiles(tile* tiles[])
{
	bool loaded = true;
	int x = 0,y = 0;
	ifstream map("map.map");
    if(map.fail())
    {
		cout << "Unable to load map file!||";
		loaded = false;
    }
	for(int i = 0;i < noTiles;++i)
	{
		int type = -1;
		map >> type;
	    if(map.fail())
        {
        cout << "Unable to load map file!||";
        loaded = false;
		break;
        }
		if((type >= 0) && (type < 3))
		{
			tiles[i] = new tile(x,y,type);
		}
		else
		{
			cout << "invalid tile type encountered!!!!||";
			loaded = false;
			break;
		}
		x += f;
		if(x >= 1080)
		{
			x = 0;
			y += f;
		}
	}
	if(loaded)
	{
		tileclips[BREAKABLE].x = 2*f;
		tileclips[BREAKABLE].y = 0;
		tileclips[BREAKABLE].w = f;
		tileclips[BREAKABLE].h = f;

		tileclips[UNBREAKABLE].x = 0;
		tileclips[UNBREAKABLE].y = 0;
		tileclips[UNBREAKABLE].w = f;
		tileclips[UNBREAKABLE].h = f;
		
		tileclips[BLANK].x = 3*f;
		tileclips[BLANK].y = 0;
		tileclips[BLANK].w = f;
		tileclips[BLANK].h = f;
	}
	map.close();
	return loaded;
}

bool iswall(SDL_Rect input1, tile* tiles[])
{
	for(int i=0;i<noTiles;++i) //go through all the tiles
    {
        if((tiles[i]->gType() == 0)||(tiles[i]->gType() == 1))
        {
            if(checktiles(input1, tiles[i]->gBlock()))
            {
				return true; //colliding
            }
		}
	}
	return false;
}

int main(int argc,char* args[])
{
	if(!init())
	{
		cout <<  "Failed to initialize!||";
	}
	
	if(!loadMedia(tileset))
	{
		cout << "Failed to load media!||";
	}
	bool quit = false;
		SDL_Event e;
		tank tank;
		SDL_RendererFlip flipType = SDL_FLIP_NONE;
			while(!quit)
			{
				while(SDL_PollEvent(&e) != 0)
				{
					if(e.type==SDL_QUIT)
					{
						quit = true;
					}
					tank.handleEvent(e,tileset);
				}

				tank.move(tileset);

				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderClear(renderer);


				for(int i = 0;i < noTiles;++i)
				{
					tileset[i]->render();
				}

				tank.render();
				TEXTure.render(910,5);

				SDL_RenderPresent(renderer);
			}
	exit(tileset);
	return 0;
}