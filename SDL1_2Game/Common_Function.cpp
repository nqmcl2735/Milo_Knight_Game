#include "Common_Function.h"

SDL_Surface* SDLCommonFunc::LoadImage(std::string file_path)
{
	SDL_Surface *load_image = NULL;
	SDL_Surface *optimize_image = NULL;
	
	load_image = IMG_Load(file_path.c_str());
	if(load_image != NULL)
	{
		optimize_image = SDL_DisplayFormat(load_image);
		SDL_FreeSurface(load_image);

		if (optimize_image != NULL) 
		{
			UINT32 color_key = SDL_MapRGB(optimize_image->format, 0, 0xFF, 0xFF);
			SDL_SetColorKey(optimize_image, SDL_SRCCOLORKEY, color_key);
		}
	}



	return optimize_image;
}

void SDLCommonFunc::ApplySurface(SDL_Surface* src, SDL_Surface* des, int x, int y)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(src, NULL, des, &offset);
}

void SDLCommonFunc::CleanUp()
{
	SDL_FreeSurface(g_screen);
	SDL_FreeSurface(g_bkground);
}
bool SDLCommonFunc::CheckPointInRect(const int & x,const int & y, const SDL_Rect& object)
{
	int lf = object.x;
	int rt = object.x + object.w;
	int tp = object.y;
	int bt = object.y + object.h;
	return (lf < x && x < rt && tp < y && y < bt);
}
bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2) 
{
	//obj1 < obj2
	std::vector<std::pair<int, int>> v;
	v.push_back(std::make_pair(object1.x, object1.y));
	v.push_back(std::make_pair(object1.x, object1.y + object1.h));
	v.push_back(std::make_pair(object1.x + object1.w, object1.y));
	v.push_back(std::make_pair(object1.x + object1.w, object1.y + object1.h)); 
	for (int i = 0; i < v.size(); i ++)
	{
		std::pair<int, int> p = v.at(i);
		if(CheckPointInRect(p.first, p.second, object2)) return 1;
	}
	//obj2 < obj1
	v.push_back(std::make_pair(object2.x, object2.y));
	v.push_back(std::make_pair(object2.x, object2.y + object2.h));
	v.push_back(std::make_pair(object2.x + object2.w, object2.y));
	v.push_back(std::make_pair(object2.x + object2.w, object2.y + object2.h)); 
	for (int i = 0; i < v.size(); i ++)
	{
		std::pair<int, int> p = v.at(i);
		if(CheckPointInRect(p.first, p.second, object1)) return 1;
	}
	return 0;
}