//done!
//level selection
//and planning

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
#include "cons.hpp"
#include "filerw.hpp"
#include "swit.hpp"

using namespace std;

const Uint8* state = SDL_GetKeyboardState(nullptr);

int cd = findc(53,2)/f;
int nM = 0;

double pointing = 0.0;
double pointing1 = 0.0;

bool rocketfired;

SDL_Rect sprite1;
SDL_Rect rect1a;
SDL_Rect rect1b;
SDL_Rect rect1c;
SDL_Rect rect1d;
SDL_Rect rocketrect;

SDL_Rect mMine1 = {findc(11,2),findc(14,2),f,f};
SDL_Rect mMine2 = {findc(19,2),findc(22,2),f,f};
SDL_Rect mMine3 = {findc(27,2),findc(30,2),f,f};
SDL_Rect switc1 = {findc(35,2),findc(38,2),f,f};
SDL_Rect switc2 = {findc(41,2),findc(44,2),f,f};
SDL_Rect switc3 = {findc(47,2),findc(50,2),f,f};
int mx1 = findc(11,2);
int my1 = findc(14,2);
int mx2 = findc(19,2);
int my2 = findc(22,2);
int mx3 = findc(27,2);
int my3 = findc(30,2);

SDL_bool mm1 = SDL_HasIntersection(&rocketrect, &mMine1);
SDL_bool mm2 = SDL_HasIntersection(&rocketrect, &mMine2);
SDL_bool mm3 = SDL_HasIntersection(&rocketrect, &mMine3);

bool init();
bool loadMedia(tile *tiles[]);
void exit(tile *tiles[]);

bool checktiles(SDL_Rect A, SDL_Rect B);

bool setTiles(tile *tiles[]);
bool iswall(SDL_Rect input1, tile* tiles[], int fType);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font *font = NULL;

SDL_Rect spriteClips[10];
aTexture TANK;
aTexture TEXTure;
aTexture TEXTure1;
aTexture BLOCKS;
aTexture scrtext;
aTexture rtext;
aTexture ROCKET;
aTexture MINE;
aTexture RFY;
aTexture RFN;
aTexture ONE;
aTexture TWON;
aTexture TWOY;
aTexture THREEN;
aTexture THREEY;
SDL_Rect tileclips[10];

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
/*	if iswall(sprite1,tiles,6)
	{
		clevel(nM);
	}*/
	clm = 0;
	clm1 = 0;
	t1 = false;
	t2 = false;
	t3 = false;
	cc1 = true;
	cc2 = true;
	cd = findc(53,2)/f;
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
	mx1 = findc(11,2);
	my1 = findc(14,2);
	mx2 = findc(19,2);
	my2 = findc(22,2);
	mx3 = findc(27,2);
	my3 = findc(30,2);
	mMine1.x = findc(11,2);
	mMine1.y = findc(14,2);
	mMine2.x = findc(19,2);
	mMine2.y = findc(22,2);
	mMine3.x = findc(27,2);
	mMine3.y = findc(30,2);
	switc1.x = findc(35,2);
	switc1.y = findc(38,2);
	switc2.x = findc(41,2);
	switc2.y = findc(44,2);
	switc3.x = findc(47,2);
	switc3.y = findc(50,2);
	pointing = 0.0;
	rPosX=startx;
	rPosY=starty;
	rocketrect.x=startx;
	rocketrect.y=starty;
	reset();
}

void tank::telrocket(SDL_Rect objct,string id)
{
	int x11 = readmapx(id);
	int y11 = readmapy(id);
	objct.x = x11;
	objct.y = y11;
	if((rocketrect.x!=sprite1.x)||(rocketrect.y!=sprite1.y))
	{
		if(sprite1.x-rocketrect.x==f||sprite1.y-rocketrect.y==f)
		{
			if(pointing1==0) //down
			{
				movement(tileset,0,1);
			}
			if(pointing1==-90) //right
			{
				movement(tileset,1,0);
			}
			if(pointing1==180)//up
			{
				movement(tileset,0,-1);
			}
			if(pointing1==90) //left
			{
				movement(tileset,-1,0);
			}
		}
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

void tank::updn(tile* tiles[])
{
	if(findc(17,1)!=0)
	{
		switch(findc(17,1))
		{
			case 40:
			if(mMine1.y == my1-4*f || mMine1.y == my1+4*f)
			{
				m = -m;
			}
			mMine1.y=mMine1.y+(m*f);
				if(iswall(mMine1,tiles,5)||iswall(mMine1,tiles,1))
				{
					frtile(tileset,'2',mMine1.x,mMine1.y);
				}
				if(iswall(mMine1,tiles,3))
				{
					telrocket(mMine1,"04");
				}
				if(iswall(mMine1,tiles,7))
				{
					telrocket(mMine1,"08");
				}
			break;
			case 80:
			if(mMine1.x == mx1-4*f || mMine1.x == mx1+4*f)
			{
				m = -m;
			}
			mMine1.x=mMine1.x+(m*f);
				if(iswall(mMine1,tiles,5)||iswall(mMine1,tiles,1))
				{
					frtile(tileset,'2',mMine1.x,mMine1.y);
				}
				if(iswall(mMine1,tiles,3))
				{
					telrocket(mMine1,"04");
				}
				if(iswall(mMine1,tiles,7))
				{
					telrocket(mMine1,"08");
				}
			break;
		}
	}
	if(findc(25,1)!=0)
	{
		switch(findc(25,1))
		{
			case 40:
			if(mMine2.y == my2-4*f || mMine2.y == my2+4*f)
			{
				m1 = -m1;
			}
			mMine2.y=mMine2.y+(m*f);
			if(iswall(mMine2,tiles,5)||iswall(mMine2,tiles,1))
			{
				frtile(tileset,'2',mMine2.x,mMine2.y);
			}
			if(iswall(mMine2,tiles,3))
			{
				telrocket(mMine2,"04");
			}
			if(iswall(mMine2,tiles,7))
			{
				telrocket(mMine2,"08");
			}

			break;
			case 80:
			if(mMine2.x == mx2-4*f || mMine2.x == mx2+4*f)
			{
				m1 = -m1;
			}
			mMine2.x=mMine2.x+(m*f);
			if(iswall(mMine2,tiles,5)||iswall(mMine2,tiles,1))
			{
				frtile(tileset,'2',mMine2.x,mMine2.y);
			}
			if(iswall(mMine2,tiles,3))
			{
				telrocket(mMine2,"04");
			}
			if(iswall(mMine2,tiles,7))
			{
				telrocket(mMine2,"08");
			}
			break;
		}
	}
	if(findc(33,1)!=0)
	{
		switch(findc(33,1))
		{
			case 40:
			if(mMine3.y == my3-4*f || mMine3.y == my3+4*f)
			{
				m2 = -m2;
			}
			mMine3.y=mMine3.y+(m*f);
			if(iswall(mMine3,tiles,5)||iswall(mMine3,tiles,1))
			{
				frtile(tileset,'2',mMine3.x,mMine3.y);
			}
			if(iswall(mMine3,tiles,3))
			{
				telrocket(mMine3,"04");
			}
			if(iswall(mMine3,tiles,7))
			{
				telrocket(mMine3,"08");
			}
			break;
			case 80:
			if(mMine3.x == mx3-4*f || mMine3.x == mx3+4*f)
			{
				m2 = -m2;
			}
			mMine3.x=mMine3.x+(m*f);
			if(iswall(mMine3,tiles,5)||iswall(mMine3,tiles,1))
			{
				frtile(tileset,'2',mMine3.x,mMine3.y);
			}
			if(iswall(mMine3,tiles,3))
			{
				telrocket(mMine3,"04");
			}
			if(iswall(mMine3,tiles,7))
			{
				telrocket(mMine3,"08");
			}
			break;
		}
	}
}

void tank::movement(tile* tiles[],int lx,int ly)
{
	if(iswall(rocketrect,tiles,0)||mm1||mm2||mm3)
	{
		rocketstop();
	}
	else if(iswall(rocketrect,tiles,5)||iswall(rocketrect,tiles,1))
	{
		if(cd>0)
		{
			frtile(tileset,'2',rocketrect.x,rocketrect.y);
			--cd;
		}
		else
		{
			rocketstop();
		}
	}
	else if(iswall(rocketrect,tiles,9))
	{
		rocketstop();
		goback();
	}
	else if(iswall(rocketrect,tiles,3))
	{
		telrocket(rocketrect,"04");
	}
	else if(iswall(rocketrect,tiles,7))
	{
		telrocket(rocketrect,"08");
	}
	else
	{
		rPosY=(ly*f)+rPosY;
		rPosX=(lx*f)+rPosX;
		rocketrect.y=(ly*f)+rocketrect.y;
		rocketrect.x=(lx*f)+rocketrect.x;
		if(iswall(rocketrect,tiles,0)||mm1||mm2||mm3)
		{
			rocketstop();
		}
		if(iswall(rocketrect,tiles,7))
		{
			telrocket(rocketrect,"08");
		}
		if(iswall(rocketrect,tiles,3))
		{
			telrocket(rocketrect,"04");
		}
		if(iswall(rocketrect,tiles,5)||iswall(rocketrect,tiles,1))
		{
			if(cd>0)
			{
				frtile(tileset,'2',rocketrect.x,rocketrect.y);
				--cd;
			}
			else
			{
				rocketstop();
			}
		}
		if(iswall(rocketrect,tiles,9))
		{
			rocketstop();
			goback();
		}
	}
}

void tank::keepmovin(tile* tiles[])
{
	if(rocketfired==true)
	{
		if(pointing1==0) //down
		{
			movement(tileset,0,1);
			movement(tileset,0,1);
		}
		if(pointing1==-90) //right
		{
			movement(tileset,1,0);
			movement(tileset,1,0);
		}
		if(pointing1==180)//up
		{
			movement(tileset,0,-1);
			movement(tileset,0,-1);
		}
		if(pointing1==90) //left
		{
			movement(tileset,-1,0);
			movement(tileset,-1,0);
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
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_UP:
			if(!(state[SDL_SCANCODE_LEFT]) && !(state[SDL_SCANCODE_RIGHT]) && !(state[SDL_SCANCODE_DOWN]))
			{
				if(iswall(rect1c,tiles,0)||iswall(rect1c,tiles,1))
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
					pointing = 180;
					keepmovin(tileset);
					updn(tileset);
				}
			}
			break;
			case SDLK_DOWN:
			if(!(state[SDL_SCANCODE_LEFT]) && !(state[SDL_SCANCODE_RIGHT]) && !(state[SDL_SCANCODE_UP]))
			{
				if(iswall(rect1d,tiles,0)||iswall(rect1d,tiles,1))
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
					pointing = 0;
					keepmovin(tileset);
					updn(tileset);
				}
			}
			break;
			case SDLK_LEFT:
			if(!(state[SDL_SCANCODE_UP]) && !(state[SDL_SCANCODE_RIGHT]) && !(state[SDL_SCANCODE_DOWN]))
			{
				if(iswall(rect1a,tiles,0)||iswall(rect1a,tiles,1))
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
					pointing = 90;
					keepmovin(tileset);
					updn(tileset);
				}
			}
			break;
			case SDLK_RIGHT:
			if(!(state[SDL_SCANCODE_LEFT]) && !(state[SDL_SCANCODE_UP]) && !(state[SDL_SCANCODE_DOWN]))
			{
				if(iswall(rect1b,tiles,0)||iswall(rect1b,tiles,1))
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
					pointing = -90;
					keepmovin(tileset);
					updn(tileset);
				}
			}
			break;
			case SDLK_SPACE:
			if(!rocketfired)
			{
				nM++;
				rocketfired = true;
				pointing1=pointing;
				if(pointing==0) //down
				{
					movement(tileset,0,1);
					movement(tileset,0,1);
				}
				if(pointing==-90) //right
				{
					movement(tileset,1,0);
					movement(tileset,1,0);
				}
				if(pointing==180)//up
				{
					movement(tileset,0,-1);
					movement(tileset,0,-1);
				}
				if(pointing==90) //left
				{
					movement(tileset,-1,0);
					movement(tileset,-1,0);
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
	SDL_bool sw1 = SDL_HasIntersection(&sprite1, &switc1);
	SDL_bool sw2 = SDL_HasIntersection(&sprite1, &switc2);
	SDL_bool sw3 = SDL_HasIntersection(&sprite1, &switc3);
	SDL_bool cd1 = SDL_HasIntersection(&rocketrect, &switc2);
	SDL_bool cd2 = SDL_HasIntersection(&rocketrect, &switc3);
	SDL_bool m11 = SDL_HasIntersection(&mMine1, &sprite1);
	SDL_bool m22 = SDL_HasIntersection(&mMine2, &sprite1);
	SDL_bool m33 = SDL_HasIntersection(&mMine3, &sprite1);
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
	}
	if((rPosX<0)||(rPosY<0)||(rPosY>520)||(rPosX>1040))
	{
		rPosX=mPosX;
		rocketrect.x=mPosX;
		rPosY=mPosY;
		rocketrect.y=mPosY;
		rocketfired=false;
	}
	if(iswall(sprite1,tiles,3))
	{
		mPosX = readmapx("04");
		mPosY = readmapy("04");
		rect1a.x = mPosX-1;
		rect1a.y = mPosY;
		rect1b.x = mPosX+f;
		rect1b.y = mPosY;
		rect1c.x = mPosX;
		rect1c.y = mPosY-1;
		rect1d.x = mPosX;
		rect1d.y = mPosY+f;
		sprite1.x = readmapx("04");
		sprite1.y = readmapy("04");
		if(!rocketfired)
		{
			rPosY = mPosY;
			rPosX = mPosX;
			rocketrect.x = rPosY;
			rocketrect.y = rPosY;
		}
	}
	if(iswall(sprite1,tiles,7))
	{
		mPosX = readmapx("08");
		mPosY = readmapy("08");
		rect1a.x = mPosX-1;
		rect1a.y = mPosY;
		rect1b.x = mPosX+f;
		rect1b.y = mPosY;
		rect1c.x = mPosX;
		rect1c.y = mPosY-1;
		rect1d.x = mPosX;
		rect1d.y = mPosY+f;
		sprite1.x = readmapx("08");
		sprite1.y = readmapy("08");
		if(!rocketfired)
		{
			rPosY = mPosY;
			rPosX = mPosX;
			rocketrect.x = rPosY;
			rocketrect.y = rPosY;
		}
	}
	while(iswall(sprite1,tiles,0)||iswall(sprite1,tiles,1)||iswall(sprite1,tiles,5)||iswall(sprite1,tiles,9))
	{
		goback();
	}
	if(iswall(sprite1,tiles,6))
	{
		goback();
	}
	if(sw1)
	{
		cout << "do stuff";
	}
	if(sw2||cd1)
	{
		if(t2 == true && cc1 == true)
		{
			t2 = false;
			cc1 = false;
			sweffects1();
		}
		else if(t2 == false && cc1 == true)
		{
			t2 = true;
			cc1 = false;
			sweffects1();
		}
		if(clm==0)
		{
			clm++;
		}
		
	}
	if(sw3||cd2)
	{
		if(t3 == true && cc2 == true)
		{
			t3 = false;
			cc2 = false;
			sweffects2();
		}
		else if(t3 == false && cc2 == true)
		{
			t3 = true;
			cc2 = false;
			sweffects2();
		}
		if(clm1==0)
		{
			clm1++;
		}
	}
	if(!sw2 && !cd1 && !cc1 && (clm>0))
	{
		cc1 = true;
	}
	if(!sw3 && !cd2 && !cc2 && (clm1>0))
	{
		cc2 = true;
	}
	if(m11||m22||m33)
	{
		goback();
	}
	if(!rocketfired && ((rPosX != mPosX)||(rPosY != mPosY)))
	{
		rocketstop();
	}
}

void tank::render()
{
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	ROCKET.render(rPosX,rPosY,&spriteClips[1],pointing1,NULL,flipType);
	TANK.render(mPosX,mPosY,&spriteClips[0],pointing,NULL,flipType);
	if(findc(17,1) != 0)
	{
		MINE.render(mMine1.x,mMine1.y,&spriteClips[2],still,NULL,flipType);
	}
	if(findc(25,1) != 0)
	{
		MINE.render(mMine2.x,mMine2.y,&spriteClips[2],still,NULL,flipType);
	}
	if(findc(33,1) != 0)
	{
		MINE.render(mMine3.x,mMine3.y,&spriteClips[2],still,NULL,flipType);
	}
	ONE.render(switc1.x,switc1.y,&spriteClips[5],still,NULL,flipType);
	if(rocketfired)
	{
		RFN.render(15*f,0,&spriteClips[4],still,NULL,flipType);
	}
	else
	{
		RFY.render(15*f,0,&spriteClips[3],still,NULL,flipType);
	}
	if(t2 == true)
	{
		TWOY.render(switc2.x,switc2.y,&spriteClips[6],still,NULL,flipType);
	}
	else
	{
		TWON.render(switc2.x,switc2.y,&spriteClips[7],still,NULL,flipType);
	}
	if(t3 == true)
	{
		THREEY.render(switc3.x,switc3.y,&spriteClips[8],still,NULL,flipType);
	}
	else
	{
		THREEN.render(switc3.x,switc3.y,&spriteClips[9],still,NULL,flipType);
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

	font = TTF_OpenFont("font.ttf",15);
	SDL_Color textColor = {0,0,0};
	string lno = "High Score: " + to_string(findc(6,4)/f);
	if(!TEXTure.loadFromRenderedText(lno, textColor))
	{
		cout << "Failed to render text!" << TTF_GetError() << "||";
		success = false;
	}
	font = TTF_OpenFont("font.ttf",20);
	string lnq = "Level: " + to_string(getlevel());
	if(!TEXTure1.loadFromRenderedText(lnq, textColor))
	{
		cout << "Failed to render text!!" << TTF_GetError() << "||";
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
	if(ONE.loadFromFile("grasses.png"))
	{
		spriteClips[5].x = 3*f;
		spriteClips[5].y = 0;
		spriteClips[5].w = f;
		spriteClips[5].h = f;
	}
	if(TWOY.loadFromFile("grasses.png"))
	{
		spriteClips[6].x = 4*f;
		spriteClips[6].y = 2*f;
		spriteClips[6].w = f;
		spriteClips[6].h = f;
	}
	if(TWON.loadFromFile("grasses.png"))
	{
		spriteClips[7].x = 0;
		spriteClips[7].y = 3*f;
		spriteClips[7].w = f;
		spriteClips[7].h = f;
	}
	if(THREEY.loadFromFile("grasses.png"))
	{
		spriteClips[8].x = f;
		spriteClips[8].y = 3*f;
		spriteClips[8].w = f;
		spriteClips[8].h = f;
	}
	if(THREEN.loadFromFile("grasses.png"))
	{
		spriteClips[9].x = 2*f;
		spriteClips[9].y = 3*f;
		spriteClips[9].w = f;
		spriteClips[9].h = f;
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
	TEXTure1.free();
	ROCKET.free();
	BLOCKS.free();
	scrtext.free();
	rtext.free();
	MINE.free();
	RFY.free();
	RFN.free();
	ONE.free();
	TWON.free();
	TWOY.free();
	THREEN.free();
	THREEY.free();
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
	Ra = A.x+f;
	Ta = A.y;
	Ba = A.y+f;

	Lb = B.x;
	Rb = B.x+f;
	Tb = B.y;
	Bb = B.y+f;
	
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
	ifstream map(lmap);
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
		if((type >= 0) && (type < 10))
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
		
		tileclips[BLANK].x = 4*f;
		tileclips[BLANK].y = 3*f;
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
		
		tileclips[TELEPORTER2a].x = 2*f;
		tileclips[TELEPORTER2a].y = f;
		tileclips[TELEPORTER2a].w = f;
		tileclips[TELEPORTER2a].h = f;

		tileclips[TELEPORTER2b].x = 4*f;
		tileclips[TELEPORTER2b].y = f;
		tileclips[TELEPORTER2b].w = f;
		tileclips[TELEPORTER2b].h = f;

		tileclips[KILYOU].x = 3*f;
		tileclips[KILYOU].y = 3*f;
		tileclips[KILYOU].w = f;
		tileclips[KILYOU].h = f;
	}
	map.close();
	return loaded;
}

bool iswall(SDL_Rect input1, tile* tiles[], int fType)
{
	for(int i=0;i<noTiles;++i) //go through all the tiles
    {
        if((tiles[i]->gType() == fType))
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
	stringstream scr;
	stringstream rl;
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
						reset();
						quit = true;
					}
					tank.handleEvent(e,tileset);
				}

				tank.move(tileset);

				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderClear(renderer);
				
				setTiles(tileset);
				scr.str(" ");
				int cm = nM;
				scr << "Moves: " << cm;
				rl.str(" ");
				int dz = cd;
				rl << "Rockets: " << dz;
				if(!scrtext.loadFromRenderedText(scr.str().c_str(), textColor))
				{
					cout << "score issue ||";
				}
				if(!rtext.loadFromRenderedText(rl.str().c_str(), textColor))
				{
					cout << "score issue ||";
				}

				for(int i = 0;i < noTiles;++i)
				{
					tileset[i]->render();
				}
				
				tank.render();
				scrtext.render(5,5);
				if((findc(6,4)/f) < 9999)
				{
					TEXTure.render(840,3);
				}
				TEXTure1.render(900,530);
				rtext.render(200,4);
				
				SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );		

				SDL_RenderPresent(renderer);
			}
	exit(tileset);
	return 0;
}