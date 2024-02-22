#include "Common_Function.h"
#include "MainObject.h"
#include "ThreatObject.h"
#include "AmoObject.h"
bool Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}
	g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	if (g_screen == NULL)
	{
		return false;
	}
	return true;
}



int main(int arc, char* argv[])
{
	
	if (Init() == false)
		return 0;

	g_bkground = SDLCommonFunc::LoadImage("bkground.png");
	if (g_bkground == NULL)
		return 0;
	SDLCommonFunc::ApplySurface(g_bkground, g_screen, 0, 0);
	//room prepare
	BaseObject room_object;
	room_object.SetRect(150, 25);
	room_object.LoadImg("room0.png");
	BaseObject border_object;
	border_object.SetRect(150, 25);
	border_object.LoadImg("border.png");
	//

	//static preapre
	BaseObject static_object;
	static_object.SetRect(100, 20);
	static_object.LoadImg("static.png");
	//
	// main prepare
	
	MainObject human_object;
	human_object.SetRect(180, 405);
	bool ret = human_object.LoadImg("knight_animsR.png");
	human_object.set_x_drc(1);
	human_object.set_y_drc(0);
	if(!ret) return 0;
	
	// threats preapre 
	
	ThreatObject* p_threats = new ThreatObject[NUM_THREAT];
	for (int t = 0; t < NUM_THREAT; t ++)
	{
		ThreatObject* p_threat = (p_threats + t);
		if(p_threat){
			p_threat->LoadImg("newDemonL.png");

			int rand_y = Random(ROOM_Y, ROOM_Y + ROOM_HEIGHT - THREAT_HEIGHT);
			p_threat->SetRect(ROOM_X + ROOM_WIDTH - THREAT_WIDTH - 10, rand_y);
			p_threat->set_x_val(-5);
			AmoObject* p_amo = new AmoObject;
			p_threat->InitAmo(p_amo);
		}
	}
	
	// game loop
	bool is_quit = false;
	while (!is_quit) 
	{
		while (SDL_PollEvent(&g_even)) 
		{	
			if (g_even.type == SDL_QUIT)
			{
				is_quit = true;
				break;
			}
			human_object.HandleInputAction(g_even);
		}
		//Aplly Background
		SDLCommonFunc::ApplySurface(g_bkground, g_screen, 0, 0);
		
		room_object.Show(g_screen);
		border_object.Show(g_screen);
		//Implement main object
		
		human_object.Show(g_screen);
		human_object.HandleMove();
		human_object.MakeAmo(g_screen);
		static_object.Show(g_screen);
		// Implement threat object
		
		for (int tt = 0; tt < NUM_THREAT; tt ++)
		{
			ThreatObject* p_threat = (p_threats + tt);
			if(p_threat){
				p_threat->Show(g_screen);
				p_threat->HandleMove(ROOM_X + ROOM_WIDTH - THREAT_WIDTH, SCREEN_HEIGHT);
				p_threat->MakeAmo(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
			}
		}
		
		
		//test in game loop
		
		//
		// Update screen	
		if (SDL_Flip(g_screen) == -1)
			return 0;
	}
	// clean up variables in program
	delete [] p_threats;
	SDLCommonFunc::CleanUp();
	SDL_Quit();
	return 1;
}