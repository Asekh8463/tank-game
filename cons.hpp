#pragma once
#include <iostream>
#include "map.hpp"

//VERY IMPORTANT!!!!!!!!!!
const int f = 40;
/* also important*/
const int noTiles = 378;
const int startx = 25*f;
const int starty = f;
const int still = 0;
const int BREAKABLE = 0;
const int UNBREAKABLE = 1;
const int BLANK = 2;
const int TELEPORTER1a = 3;
const int TELEPORTER1b = 4;
const int STATIC_MINE = 5;
const int EXIT = 6;
const int TELEPORTER2a = 7;
const int TELEPORTER2b = 8;
const int KILYOU = 9;
//not constants but theyre here anyways
int m = -2;
int m1 = -2;
int m2 = -2;
int clm = 0;
int clm1 = 0;
bool rocketfired;
//switches and mines, idk
bool t1 = false;
bool t2 = false;
bool t3 = false;
bool cc1 = true;
bool cc2 = true;
//rects
SDL_Rect sprite1;
SDL_Rect rect1a;
SDL_Rect rect1b;
SDL_Rect rect1c;
SDL_Rect rect1d;
SDL_Rect rocketrect;
//misc
const char isc = 'C';
tile* tileset[noTiles];