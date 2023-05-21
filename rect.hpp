#pragma once
#include <SDL.h>
#include <iostream>

#include "cons.hpp"
#include "filerw.hpp"

int mx1 = findc(11,2);
int my1 = findc(14,2);
int mx2 = findc(19,2);
int my2 = findc(22,2);
int mx3 = findc(27,2);
int my3 = findc(30,2);

int cd = findc(53,2)/f;

SDL_Rect mMine1 = {findc(11,2),findc(14,2),f,f};
SDL_Rect mMine2 = {findc(19,2),findc(22,2),f,f};
SDL_Rect mMine3 = {findc(27,2),findc(30,2),f,f};
SDL_Rect switc1 = {findc(35,2),findc(38,2),f,f};
SDL_Rect switc2 = {findc(41,2),findc(44,2),f,f};
SDL_Rect switc3 = {findc(47,2),findc(50,2),f,f};

SDL_Rect l1 = {switc2.x-f,switc2.y,f,f};
SDL_Rect r1 = {switc2.x+f,switc2.y,f,f};
SDL_Rect d1 = {switc2.x,switc2.y+f,f,f};
SDL_Rect u1 = {switc2.x,switc2.y-f,f,f};

SDL_Rect l2 = {switc3.x-f,switc3.y,f,f};
SDL_Rect r2 = {switc3.x+f,switc3.y,f,f};
SDL_Rect d2 = {switc3.x,switc3.y+f,f,f};
SDL_Rect u2 = {switc3.x,switc3.y-f,f,f};