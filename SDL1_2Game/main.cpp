#include "Common_Function.h"
#include "MainObject.h"
#include "ThreatObject.h"
#include "AmoObject.h"
#include <time.h>
#include <vector>
#include "ImpTimer.h" 
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
struct round 
{
	// room objects
	BaseObject room_object;
	BaseObject border_object;
	BaseObject Lgate_object;
	BaseObject Rgate_object;
	// static object
	BaseObject static_object;
	// main object
	MainObject human_object;
	std::vector<ThreatObject*> p_threats;
	void preapre()
	{
		room_object.SetRect(150, 25);
		room_object.LoadImg("room0.png");
		border_object.SetRect(150, 25);
		border_object.LoadImg("border.png");
		Lgate_object.SetRect(150, 335);
		Lgate_object.SetWidthHeight(25, 145);
		Lgate_object.LoadImg("Gate.png");
		Rgate_object.SetRect(1025, 335);
		Rgate_object.SetWidthHeight(25, 145);
		Rgate_object.LoadImg("Gate.png");

		static_object.SetRect(100, 20);
		static_object.LoadImg("static.png");
		
		human_object.SetRect(180, 330);
		human_object.LoadImg("knight_animsR.png");
		human_object.set_x_drc(1);
		human_object.set_y_drc(0);

		for (int t = 0; t < NUM_THREAT; t ++)
		{
			ThreatObject *p_threat = new ThreatObject;
			p_threat->LoadImg("newDemonL.png");
			int rand_y = Random(ROOM_Y, ROOM_Y + ROOM_HEIGHT - THREAT_HEIGHT);
			p_threat->SetRect(ROOM_X + ROOM_WIDTH - THREAT_WIDTH - 10, rand_y);
			p_threat->set_x_val(-5); 
			p_threats.push_back(p_threat);
		}
	}
	void gameLoop(bool &is_quit, bool &p_still_live, long long numLoop)
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
		Lgate_object.Show(g_screen);
		Rgate_object.Show(g_screen);
		static_object.Show(g_screen);
		//Implement main object
		human_object.Show(g_screen);
		human_object.HandleMove(Lgate_object, Rgate_object);
		human_object.MakeAmo(g_screen);
		human_object.coolProcess();
		

		//xu li va cham dan vs main
		
		for (int i = 0; i < p_threats.size(); i ++)
		{
			ThreatObject *p_threat = p_threats.at(i);
			if(p_threat->get_is_alive()){
				std::vector<AmoObject*> amo_list = p_threat->GetAmoList();
				for (int am = 0; am < amo_list.size(); am ++)
				{
					AmoObject* p_amo = amo_list.at(am);
					if(p_amo && p_amo -> get_is_move())
					{
						bool is_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), human_object.GetRect());
						if(is_col)
						{
							if(human_object.get_shield() > 0)
							{
								int p_shield = human_object.get_shield();
								human_object.set_shield(max(p_shield - p_threat->get_pow(), 0));
							}
							else 
							{
								human_object.change_health((-1) * (100.0/100.0 * p_threat->get_pow()));
							}
							if(human_object.get_health() <= 0){
								p_still_live = 0;
								is_quit = 1;
								break;
							}
							if(am < amo_list.size()) amo_list.erase(amo_list.begin() + am);
							p_threat->SetAmoList(amo_list);
						}
					}
				}
				if(p_still_live == 0) return;
			}
		}
		if(p_still_live == 0) return;
		//

		// Implement threat object
		for (int i = 0; i < p_threats.size(); i ++)
		{
			std::vector<ThreatObject*> threat_list = p_threats;
			ThreatObject *p_threat = threat_list.at(i);
			if(p_threat->get_is_alive()){
				p_threat->Show(g_screen);
				
				if(numLoop % 24 == 0) {
					p_threat-> set_x_val(Random(-1, 1) * (5));
					p_threat-> set_y_val(Random(-1, 1) * (5));
					
					if(p_threat->get_x_val() == 5){ 
						p_threat->LoadImg("NewDemonR.png");
						p_threat->set_LR(1);
					}
					else if (p_threat->get_x_val() == -5){ 
						p_threat->LoadImg("NewDemonL.png");
						p_threat->set_LR(-1);
					}
				}
				if(numLoop % 20 == 0){ 
					AmoObject* p_amo = new AmoObject;
					p_threat->InitAmo(p_amo);
				}

				p_threat->HandleMove(ROOM_X + ROOM_WIDTH - THREAT_WIDTH, SCREEN_HEIGHT);
				p_threat->MakeAmo(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				// xu li va cham dan vs threat
				if(!p_threat->checkAmour(human_object))
				{
					p_threat->set_is_alive(0);
					p_threat->SetRect(SCREEN_WIDTH, SCREEN_HEIGHT);
				}
			}
		}
		
		
		//test in game loop
		
		//
	}
}R1;


int main(int arc, char* argv[])
{

	ImpTimer fps_timer;

	srand(time(NULL));
	if (Init() == false)
		return 0;

	g_bkground = SDLCommonFunc::LoadImage("bkground.png");
	if (g_bkground == NULL)
		return 0;
	SDLCommonFunc::ApplySurface(g_bkground, g_screen, 0, 0);
	R1.preapre();
	bool is_quit = false;
	bool p_still_live = 1;
	long long numLoop = 0;
	while (!is_quit) 
	{
		fps_timer.start();
		R1.gameLoop(is_quit, p_still_live, numLoop);
		// Update screen	
		if (SDL_Flip(g_screen) == -1)
			return 0;
		
		int real_imp_time = fps_timer.get_tick();
		int time_one_frame = 1000/FRAME_PER_SECOND;
		if(real_imp_time < time_one_frame)
		{
 			int delay_time = time_one_frame - real_imp_time;
			SDL_Delay(delay_time);
		}
		numLoop ++;
	}
	// clean up variables in program
	R1.p_threats.clear();
	SDLCommonFunc::CleanUp();
	SDL_Quit();
	return 1;
}