#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <Windows.h>
#include <SDL.h>
#include <string>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>
#define Random(lf, rt) (lf + rand() % (rt - lf + 1))

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int SCREEN_BPP = 32;

const int NUM_THREAT = 1;

const int ROOM_WIDTH = 861;
const int ROOM_HEIGHT = 707;
const int ROOM_X = 170;
const int ROOM_Y = 45;
const int GATE_TOP = 330;
const int GATE_BOT = 405;


const int FRAME_PER_SECOND = 40; // fps cua game


static SDL_Surface *g_screen = NULL;
static SDL_Surface *g_bkground = NULL;

static SDL_Event g_even;

namespace SDLCommonFunc
{
	
	SDL_Surface* LoadImage(std::string file_path);
	void ApplySurface(SDL_Surface* src, SDL_Surface* des, int x, int y);
	bool CheckPointInRect(const int & x,const int & y, const SDL_Rect& object);
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
	void CleanUp();
}


#endif