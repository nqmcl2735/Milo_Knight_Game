#ifndef HOME_OBJECT_H_ 
#define HOME_OBJECT_H_ 

#include "BaseObject.h" 
#include "Button.h" 
#include "MainObject.h"
#include "Button.h"

class Home 
{
public : 
	void Prepare (MainObject & human_object) 
	{
	// set rect for human object 
	human_object.SetRect (SCREEN_WIDTH /2 , SCREEN_HEIGHT /2 ) ; 
	human_object.set_x_drc(1) ; 
	human_object.set_y_drc(0) ; 
	//home 
	room.SetRect (150,115) ; 
	room.LoadImg ("House_IMG.png") ; 
	Lborder.SetRect (0,0) ; 
	Lborder.SetWidthHeight(150,SCREEN_HEIGHT) ;
	Lborder.LoadImg("Obstacles.png")  ;
	Rborder.SetRect (1050,0) ; 
	Rborder.SetWidthHeight(150,SCREEN_HEIGHT) ;
	Rborder.LoadImg("Obstacles.png") ;
	Tborder.SetRect(0,0) ; 
	Tborder.SetWidthHeight (SCREEN_WIDTH, 400) ; 
	Tborder.LoadImg("Obstacles.png") ; 
	Bborder.SetRect (1050,685) ; 
	Bborder.SetWidthHeight (SCREEN_WIDTH,115) ;
	Bborder.LoadImg("Obstacles.png"); 
	}
	void Show_Home (bool &is_quit ,bool & p_still_live, SDL_Surface * des , MainObject & human_object)  
		{
	while (SDL_PollEvent(&g_even)) 
	{
		if (g_even.type == SDL_QUIT) 
		{
			is_quit = true ; 
			break ; 
		}
		human_object.HandleInputAction (g_even) ; 
	}
		//Aplly Background
		SDLCommonFunc::ApplySurface(g_bkground, des, 0, 0);
		room.Show(des) ; 
		Lborder.Show(des ) ; 
		Rborder.Show(des ) ; 
		Tborder.Show(des ) ; 
		Bborder.Show(des ) ; 
		//Implement main object
		human_object.Show(des);
		if (SDLCommonFunc :: CheckCollision (human_object.GetRect(),Lborder.GetRect())|| 
			SDLCommonFunc :: CheckCollision (human_object.GetRect(),Rborder.GetRect())||
			SDLCommonFunc :: CheckCollision (human_object.GetRect(),Tborder.GetRect())||
			SDLCommonFunc :: CheckCollision (human_object.GetRect(),Bborder.GetRect()))
		{
			human_object.set_in_obstacles(true) ; 
		}
		else human_object.set_in_obstacles(false) ; 
		human_object.HandleMoveInHome(); 
		human_object.Show_Static(des,g_font_text) ;
}
protected : 
	BaseObject Lborder ; 
	BaseObject Rborder ; 
	BaseObject Tborder ; 
	BaseObject Bborder ; 
	BaseObject room ; 
	BaseObject door ; 
};

#endif 