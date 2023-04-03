//TODO:
//rocket explosion rect
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
#include <limits>

#include "map.hpp"
#include "texture.hpp"
#include "tank.hpp"

using namespace std;

int level = 1;

const Uint8* state = SDL_GetKeyboardState(nullptr);

int m = -1;
int nM = 0;
const int f = 40;
const int noTiles = 378;
const int startx = 25*f;
const int starty = f;

const double still = 0.0;
double pointing = 0.0;
double pointing1 = 0.0;

bool rocketfired;

SDL_Rect sprite1;
SDL_Rect rect1a;
SDL_Rect rect1b;
SDL_Rect rect1c;
SDL_Rect rect1d;
SDL_Rect rocketrect;
SDL_Rect exprect;


SDL_Rect mMine1 = {f,10*f,f,f};

//tile types
const int BREAKABLE = 0;
const int UNBREAKABLE = 1;
const int BLANK = 2;
const int TELEPORTER1a = 3;
const int TELEPORTER1b = 4;
const int STATIC_MINE = 5;
const int EXIT = 6;
/*const int TELEPORTER2a = 5;
const int TELEPORTER2b = 6;
const int TELEPORTER3a = 7;
const int TELEPORTER3b = 8;
const int TELEPORTER4a = 9;
const int TELEPORTER4b = 10;*/

tile* tileset[noTiles];

bool init();
bool loadMedia(tile *tiles[]);
void exit(tile *tiles[]);

bool checktiles(SDL_Rect A, SDL_Rect B);

bool setTiles(tile *tiles[]);
bool iswall(SDL_Rect input1, tile* tiles[]);
bool isteleport1(SDL_Rect input2, tile* tiles[]);
bool ismine(SDL_Rect input3, tile* tiles[]);
bool reachedend(SDL_Rect input4, tile* tiles[]);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font *font = NULL;

SDL_Rect spriteClips[5];
aTexture TANK;
aTexture TEXTure;
aTexture BLOCKS;
aTexture scrtext;
aTexture ROCKET;
aTexture MINE;
aTexture RFY;
aTexture RFN;
SDL_Rect tileclips[7];

int readmapy()
{
	unsigned int wline = 0;
	int aX = -1;
	string usc;
	ifstream mapFile("map.map");
	while(getline(mapFile, usc))
	{
		wline++;
		if(usc.find("04") != string::npos)
		{
			aX = wline;
		}
	}
	mapFile.close();
	return aX;
}

int readmapx()
{
	int aY;
	string usc = "04";
	string datas;
	ifstream mapFile("map.map");
	for(int i=1; i <= readmapy(); i++)
	{
		getline(mapFile, datas);
	}
	size_t loc = datas.find("04");
	if(loc != string::npos)
	{
		aY = loc;
	}
	else
	{
		aY = -1;
	}
	mapFile.close();
	return aY;
}

SDL_Rect telexit = {(((readmapx())/3)*f),(readmapy()*f)-f,f,f};

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
	nM = 0;
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
	mMine1.x = f;
	mMine1.y = 10*f;
	pointing = 0.0;
	exprect.x = startx-f;
	exprect.y = starty-f;
	rocketstop();
}

void tank::telrocket()
{
	rPosX = telexit.x;
	rPosY = telexit.y;
	rocketrect.x = rPosX;
	rocketrect.y = rPosY;
	exprect.x = telexit.x-f;
	exprect.y = telexit.y-f;
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
	rPosX = mPosX;
	rPosY = starty;
	//setting rects start pos
	rocketrect.x = mPosX;
	rocketrect.y = mPosY;
	rocketrect.w = f;
	rocketrect.h = f;
	sprite1.x=mPosX;
	sprite1.y=mPosY;
	sprite1.w=f;
	sprite1.h=f;
	exprect.x = mPosX-f;
	exprect.y = mPosY-f;
	exprect.w = 3*f;
	exprect.h = 3*f;
}

void tank::updn()
{
	switch(mMine1.y)
	{
	case 7*f:
		m=-m;
		mMine1.y=7*f + (m*f);
		break;
	case 8*f:
		mMine1.y=8*f + (m*f);
		break;
	case 9*f:
		mMine1.y=9*f + (m*f);
		break;
	case 10*f:
		mMine1.y=10*f + (m*f);
		break;
	case 11*f:
		mMine1.y=11*f + (m*f);
		break;
	case 12*f:
		mMine1.y=12*f + (m*f);
		break;
	case 13*f:
		m=-m;
		mMine1.y=13*f + (m*f);
		break;
	}
}

void tank::keepmovin(tile* tiles[])
{
	SDL_bool colliding = SDL_HasIntersection(&mMine1,&sprite1);
	if(rocketfired==true)
	{
		if(pointing1==0) //down
		{
			rPosY=rPosY+f;
			rocketrect.y=rocketrect.y+f;
			exprect.y=exprect.y+f;
			if(iswall(rocketrect,tiles))
			{
				rocketstop();
			}
			else if(isteleport1(rocketrect, tiles))
			{
				telrocket();
				rPosY=rPosY+f;
				rocketrect.y=rocketrect.y+f;
				exprect.y=exprect.y+f;
			}
			else
			{
				rPosY=rPosY+f;
				rocketrect.y=rocketrect.y+f;
				exprect.y=exprect.y+f;
			}
		}
		if(pointing1==-90) //right
		{
			rPosX=rPosX+f;
			rocketrect.x=rocketrect.x+f;
			exprect.x=exprect.x+f;
			if(iswall(rocketrect,tiles))
			{
				rocketstop();
			}
			else if(isteleport1(rocketrect, tiles))
			{
				telrocket();
				rPosX=rPosX+f;
				rocketrect.x=rocketrect.x+f;
				exprect.x=exprect.x+f;
			}
			else
			{
				rPosX=rPosX+f;
				rocketrect.x=rocketrect.x+f;
				exprect.x=exprect.x+f;
			}
		}
		if(pointing1==180)//up
		{
			rPosY=rPosY-f;
			rocketrect.y=rocketrect.y-f;
			exprect.y=exprect.y-f;
			if(iswall(rocketrect,tiles))
			{
				rocketstop();
			}
			else if(isteleport1(rocketrect, tiles))
			{
				telrocket();
				rPosY=rPosY-f;
				rocketrect.y=rocketrect.y-f;
				exprect.y=exprect.y-f;
			}
			else
			{
				rPosY=rPosY-f;
				rocketrect.y=rocketrect.y-f;
				exprect.y=exprect.y-f;
			}
		}
		if(pointing1==90) //left
		{
			rPosX=rPosX-f;
			rocketrect.x=rocketrect.x-f;
			exprect.x=exprect.x-f;
			if(iswall(rocketrect,tiles))
			{
				rocketstop();
			}
			else if(isteleport1(rocketrect, tiles))
			{
				telrocket();
				rPosX=rPosX-f;
				rocketrect.x=rocketrect.x-f;
				exprect.x=exprect.x-f;
			}
			else
			{
				rPosX=rPosX-f;
				rocketrect.x=rocketrect.x-f;
				exprect.x=exprect.x-f;
			}
		}
	}
	else
	{
		rPosX=mPosX;
		rPosY=mPosY;
		rocketrect.x = rPosX;
		rocketrect.y = rPosY;
		exprect.x = rPosX-f;
		exprect.y = rPosY-f;
	}
	if(colliding)
	{
		goback();
	}
}

void tank::handleEvent(SDL_Event &e, tile* tiles[])
{
	SDL_bool collidin = SDL_HasIntersection(&sprite1,&exprect);
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_UP:
			if(!(state[SDL_SCANCODE_LEFT]) && !(state[SDL_SCANCODE_RIGHT]) && !(state[SDL_SCANCODE_DOWN]))
			{
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
					nM++;
					mPosY = mPosY-f;
					rect1a.y = rect1a.y-f;
					rect1b.y = rect1b.y-f;
					rect1c.y = rect1c.y-f;
					rect1d.y = rect1d.y-f;
					sprite1.y = sprite1.y-f;
					if(!rocketfired)
					{
						exprect.y=exprect.y-f;
					}
					pointing = 180;
					keepmovin(tileset);
					updn();
				}
			}
			break;
			case SDLK_DOWN:
			if(!(state[SDL_SCANCODE_LEFT]) && !(state[SDL_SCANCODE_RIGHT]) && !(state[SDL_SCANCODE_UP]))
			{
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
					nM++;
					mPosY = mPosY+f;
					rect1a.y = rect1a.y+f;
					rect1b.y = rect1b.y+f;
					rect1c.y = rect1c.y+f;
					rect1d.y = rect1d.y+f;
					sprite1.y = sprite1.y+f;
					if(!rocketfired)
					{
						exprect.y=exprect.y+f;
					}
					pointing = 0;
					keepmovin(tileset);
					updn();
				}
			}
			break;
			case SDLK_LEFT:
			if(!(state[SDL_SCANCODE_UP]) && !(state[SDL_SCANCODE_RIGHT]) && !(state[SDL_SCANCODE_DOWN]))
			{
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
					nM++;
					mPosX = mPosX-f;
					rect1a.x = rect1a.x-f;
					rect1b.x = rect1b.x-f;
					rect1c.x = rect1c.x-f;
					rect1d.x = rect1d.x-f;
					sprite1.x = sprite1.x-f;
					if(!rocketfired)
					{
						exprect.x=exprect.x-f;
					}
					pointing = 90;
					keepmovin(tileset);
					updn();
				}
			}
			break;
			case SDLK_RIGHT:
			if(!(state[SDL_SCANCODE_LEFT]) && !(state[SDL_SCANCODE_UP]) && !(state[SDL_SCANCODE_DOWN]))
			{
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
					nM++;
					mPosX = mPosX+f;
					rect1a.x = rect1a.x+f;
					rect1b.x = rect1b.x+f;
					rect1c.x = rect1c.x+f;
					rect1d.x = rect1d.x+f;
					sprite1.x = sprite1.x+f;
					if(!rocketfired)
					{
						exprect.x=exprect.x+f;
					}
					pointing = -90;
					keepmovin(tileset);
					updn();
				}
			}
			break;
			case SDLK_SPACE:
			nM++;
			if(!rocketfired)
			{
				rocketfired = true;
				if(pointing == 0) //facing down
				{
					pointing1 = 0;
					rPosY = rPosY+f;
					rocketrect.y=rocketrect.y+f;
					exprect.y=exprect.y+f;
					if(iswall(rocketrect,tiles))
					{
						if(collidin)
						{
							goback();
						}
					rocketstop();
					}
					else if(isteleport1(rocketrect,tiles))
					{
						telrocket();
						rPosY = rPosY+f;
						rocketrect.y=rocketrect.y+f;
						exprect.y=exprect.y+f;
					}
					else
					{
						rPosY = rPosY+f; //keep moving
						rocketrect.y=rocketrect.y+f;
						exprect.y=exprect.y+f;
					}
				}
				else if(pointing == 90) //facing right
				{
					pointing1 = 90;
					rPosX = rPosX-f;
					rocketrect.x=rocketrect.x-f;
					exprect.x=exprect.x-f;
					if(iswall(rocketrect,tiles))
					{
						if(collidin)
						{
							goback();
						}
						rocketstop();
					}
					else if(isteleport1(rocketrect,tiles))
					{
						telrocket();
						rPosX = rPosX-f;
						rocketrect.x=rocketrect.x-f;
						exprect.x=exprect.x-f;
					}
					else
					{
						rPosX = rPosX-f; //keep moving
						rocketrect.x=rocketrect.x-f;
						exprect.x=exprect.x-f;
					}
				}
				else if(pointing == 180)//facing up
				{
					pointing1 = 180;
					rPosY = rPosY-f;
					rocketrect.y=rocketrect.y-f;
					exprect.y=exprect.y-f;
					if(iswall(rocketrect,tiles))
					{
						if(collidin)
						{
							goback();
						}
						rocketstop();
					}
					else if(isteleport1(rocketrect,tiles))
					{
						telrocket();
						rPosY = rPosY-f;
						rocketrect.y=rocketrect.y-f;
						exprect.y=exprect.y-f;
					}
					else
					{
						rPosY = rPosY-f; //keep moving
						rocketrect.y=rocketrect.y-f;
						exprect.y=exprect.y-f;
					}
				}
				else //facing left
				{
					pointing1 = -90;
					rPosX = rPosX+f;
					rocketrect.x=rocketrect.x+f;
					exprect.x=exprect.x+f;
					if(iswall(rocketrect,tiles))
					{
						if(collidin)
						{
							goback();
						}
						rocketstop();
					}
					else if(isteleport1(rocketrect,tiles))
					{
						telrocket();
						rPosX = rPosX+f;
						rocketrect.x=rocketrect.x+f;
						exprect.x= exprect.x+f;
					}
					else
					{
						rPosX = rPosX+f; //keep moving
						rocketrect.x=rocketrect.x+f;
						exprect.x= exprect.x+f;
					}
				}
			}
			break;
			case SDLK_r:
			goback();
			break;
		}
	}
}

void tank::move(tile* tiles[])
{
	if(mPosX<0) //too far left
	{
		--nM;
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
		mMine1.y = mMine1.y - (m*f);
	}
	if(mPosY<0) //too far up
	{
		--nM;
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
		mMine1.y = mMine1.y - (m*f);
	}
	if(mPosX>1040) // too far right
	{
		--nM;
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
		mMine1.y = mMine1.y - (m*f);
	}
	if(mPosY>520) //too far down
	{
		--nM;
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
		mMine1.y = mMine1.y - (m*f);
	}
	if((rPosX<0)||(rPosY<0)||(rPosY>520)||(rPosX>1040))
	{
		rPosX=mPosX;
		rocketrect.x=mPosX;
		rPosY=mPosY;
		rocketrect.y=mPosY;
		rocketfired=false;
	}
	if(iswall(rocketrect,tiles))
	{
		rocketstop();
	}
	if(isteleport1(sprite1, tiles))
	{
		mPosX = telexit.x;
		mPosY = telexit.y;
		rect1a.x = mPosX-1;
		rect1a.y = mPosY;
		rect1b.x = mPosX+f;
		rect1b.y = mPosY;
		rect1c.x = mPosX;
		rect1c.y = mPosY-1;
		rect1d.x = mPosX;
		rect1d.y = mPosY+f;
		sprite1.x = telexit.x;
		sprite1.y = telexit.y;
		if(!rocketfired)
		{
			rPosY = mPosY;
			rPosX = mPosX;
			rocketrect.x = rPosY;
			rocketrect.y = rPosY;
		}
	}
	while(ismine(sprite1, tiles))
	{
		goback();
	}
	if(isteleport1(rocketrect, tiles))
	{
		telrocket();
	}
	if(reachedend(sprite1, tiles))
	{
		goback();
		level++;
	}
}

void tank::render()
{
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	ROCKET.render(rPosX,rPosY,&spriteClips[1],pointing1,NULL,flipType);
	TANK.render(mPosX,mPosY,&spriteClips[0],pointing,NULL,flipType);
	MINE.render(mMine1.x,mMine1.y,&spriteClips[2],still,NULL,flipType);
	if(rocketfired)
	{
	RFN.render(13*f,0,&spriteClips[4],still,NULL,flipType);
	}
	else
	{
	RFY.render(13*f,0,&spriteClips[3],still,NULL,flipType);
	}
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

	if(!TEXTure.loadFromRenderedText("Level: 1", textColor))
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

	if(TANK.loadFromFile("grasses.png"))
	{
		spriteClips[0].x = f;
		spriteClips[0].y = 0;
		spriteClips[0].w = f;
		spriteClips[0].h = f;
	}
	if(ROCKET.loadFromFile("grasses.png"))
	{
		spriteClips[1].x = 0;
		spriteClips[1].y = f;
		spriteClips[1].w = f;
		spriteClips[1].h = f;
	}
	if(MINE.loadFromFile("grasses.png"))
	{
		spriteClips[2].x = 0;
		spriteClips[2].y = 2*f;
		spriteClips[2].w = f;
		spriteClips[2].h = f;
	}
	if(RFY.loadFromFile("grasses.png"))
	{
		spriteClips[3].x = 2*f;
		spriteClips[3].y = 2*f;
		spriteClips[3].w = f;
		spriteClips[3].h = f;
	}
	if(RFN.loadFromFile("grasses.png"))
	{
		spriteClips[4].x = 3*f;
		spriteClips[4].y = 2*f;
		spriteClips[4].w = f;
		spriteClips[4].h = f;
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
	scrtext.free();
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
		if((type >= 0) && (type < 7))
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
		
		tileclips[TELEPORTER1a].x = 4*f;
		tileclips[TELEPORTER1a].y = 0;
		tileclips[TELEPORTER1a].w = f;
		tileclips[TELEPORTER1a].h = f;

		tileclips[TELEPORTER1b].x = f;
		tileclips[TELEPORTER1b].y = f;
		tileclips[TELEPORTER1b].w = f;
		tileclips[TELEPORTER1b].h = f;

		tileclips[STATIC_MINE].x = 3*f;
		tileclips[STATIC_MINE].y = f;
		tileclips[STATIC_MINE].w = f;
		tileclips[STATIC_MINE].h = f;

		tileclips[EXIT].x = f;
		tileclips[EXIT].y = 2*f;
		tileclips[EXIT].w = f;
		tileclips[EXIT].h = f;
/*
		tileclips[TELEPORTER3a].x = 4*f;
		tileclips[TELEPORTER3a].y = 0;
		tileclips[TELEPORTER3a].w = f;
		tileclips[TELEPORTER3a].h = f;

		tileclips[TELEPORTER3b].x = f;
		tileclips[TELEPORTER3b].y = f;
		tileclips[TELEPORTER3b].w = f;
		tileclips[TELEPORTER3b].h = f;

		tileclips[TELEPORTER4a].x = 4*f;
		tileclips[TELEPORTER4a].y = 0;
		tileclips[TELEPORTER4a].w = f;
		tileclips[TELEPORTER4a].h = f;

		tileclips[TELEPORTER4b].x = f;
		tileclips[TELEPORTER4b].y = f;
		tileclips[TELEPORTER4b].w = f;
		tileclips[TELEPORTER4b].h = f; */
	}
	map.close();
	return loaded;
}

bool iswall(SDL_Rect input1, tile* tiles[])
{
	for(int i=0;i<noTiles;++i) //go through all the tiles
    {
        if((tiles[i]->gType() < 2))
        {
            if(checktiles(input1, tiles[i]->gBlock()))
            {
				return true; //colliding
            }
		}
	}
	return false;
}

bool isteleport1(SDL_Rect input2, tile* tiles[])
{
	for(int i=0;i<noTiles;++i) //go through all the tiles
    {
        if((tiles[i]->gType() == 3))
        {
            if(checktiles(input2, tiles[i]->gBlock()))
            {
				return true; //colliding
            }
		}
	}
	return false;
}

bool ismine(SDL_Rect input3, tile* tiles[])
{
	for(int i=0;i<noTiles;++i)
	{
		if((tiles[i]->gType() == 5))
		{
			if(checktiles(input3, tiles[i]->gBlock()))
			{
				return true;
			}
		}
	}
	return false;
}

bool reachedend(SDL_Rect input4, tile* tiles[])
{
	for(int i=0;i<noTiles;++i)
	{
		if((tiles[i]->gType() == 6))
		{
			if(checktiles(input4, tiles[i]->gBlock()))
			{
				return true;
			}
		}
	}
	return false;
}

int main(int argc,char* args[])
{
	stringstream scr;
	SDL_Color textColor = {0,0,0,255};

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
				
				scr.str(" ");
				int cm = nM;
				scr << "Moves: " << cm;
				if(!scrtext.loadFromRenderedText(scr.str().c_str(), textColor))
				{
					cout << "score issue ||" << endl;
				}

				for(int i = 0;i < noTiles;++i)
				{
					tileset[i]->render();
				}
				
				tank.render();
				scrtext.render(5,5);
				TEXTure.render(910,5);
				SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );		
				SDL_RenderDrawRect( renderer, &exprect );

				SDL_RenderPresent(renderer);
			}
	exit(tileset);
	return 0;
}