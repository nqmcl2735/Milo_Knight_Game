#include "Common_Function.h"
#include "MainObject.h"
#include "ThreatObject.h"
#include "AmoObject.h"
#include <time.h>
#include <vector>
#include "ImpTimer.h" 
#include "TextObject.h"
#include "UserInterface.h"
#include <fstream>
TTF_Font*  g_font_text = NULL;
MainObject human_object;

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
	if(TTF_Init() == -1)
	{
		return false;
	}
	g_font_text = TTF_OpenFont("Mojang-Bold.ttf", 30);
	if (g_font_text == NULL) 
	{
		return false;
	}
	return true;
}


struct menuStruct 
{
	TTF_Font* menu_font;
	static const int numItems = 3;
	BaseObject menuBk;
	BaseObject menuItems[numItems];
	BaseObject menuArrow;
	std::string menuString[numItems];
	TextObject menuText[numItems];
	BaseObject guideBk;
	
} menu;




enum gameStatus 
{
	START,
	PLAYING,
	PAUSE,
	GUIDE
};
int g_stat = START;




struct round 
{
	enum round_lvl
	{
		R1,
		R2,
		R3,
		R4,
		R5
	};
	enum round_status
	{
		BEFORE,
		RUNNING,
		DONEROUND
	};
	
	int r_mark;
	int r_stat;
	int r_lvl;
	round(int level)
	{
		r_mark = 0;
		r_lvl = level;
		r_stat = BEFORE;
	}
	// room objects
	BaseObject room_object;
	BaseObject border_object;
	BaseObject Lgate_object;
	BaseObject Rgate_object;
	BaseObject road_object;
	
	std::vector<ThreatObject*> p_threats;

	//test outside game loop
	void Move(BaseObject & gate, int dir)
	{
		SDL_Rect tmp = gate.GetRect();
		tmp.y += 5 * dir;
		gate.SetRect(tmp.x, tmp.y);
	}
	void before_frame()
	{
		SDL_Rect tmp = Lgate_object.GetRect();
		if(tmp.y >= 165) Move(Lgate_object, -1);
	}
	void running_frame()
	{
		SDL_Rect tmp = Lgate_object.GetRect();
		if(tmp.y < 335) Move(Lgate_object, 1);
	}
	void doneround_frame()
	{
		SDL_Rect tmp = Rgate_object.GetRect();
		if(tmp.y >= 165) Move(Rgate_object, -1);
	}
	void include_threat()
	{
		for (int t = 0; t < NUM_THREAT; t ++)
		{
			ThreatObject *p_threat = new ThreatObject;
			p_threat->LoadImg("newDemonL.png");
			int rand_y = Random(ROOM_Y, ROOM_Y + ROOM_HEIGHT - THREAT_HEIGHT);
			p_threat->SetRect(ROOM_X + ROOM_WIDTH - THREAT_WIDTH - 10, rand_y);
			p_threats.push_back(p_threat);
		}
	}
	void cleanup_threat()
	{
		p_threats.clear();
	}
	
	//
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
		road_object.SetRect(0, 310);
		road_object.LoadImg("road.png");
		


		human_object.SetRect(0, 330);
		human_object.LoadImg("knight_animsR.png");
		human_object.set_x_drc(1);
		human_object.set_y_drc(0);
		human_object.prepare();
		
	}

	void gameLoop(bool &is_quit, bool &p_still_live, long long numLoop)
	{
		
		//update round_status
		if(r_stat == BEFORE)
		{
			SDL_Rect tmp = human_object.GetRect();
			if(tmp.x >= 180){ 
				r_stat = RUNNING;
				include_threat();
			}
		}
		else if(r_stat == RUNNING)
		{
			if(r_mark == NUM_THREAT)
			{
				cleanup_threat();
				r_stat = DONEROUND;
			}
		}
		// apply frame
		if(r_stat == BEFORE) 
			before_frame();
		else if(r_stat == RUNNING)
			running_frame();
		else if(r_stat = DONEROUND)
			doneround_frame();
		//Aplly Background
		SDLCommonFunc::ApplySurface(g_bkground, g_screen, 0, 0);
		road_object.Show(g_screen);
		room_object.Show(g_screen);
		border_object.Show(g_screen);
		Lgate_object.Show(g_screen);
		Rgate_object.Show(g_screen);
		
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
					if(p_still_live == 0) return;
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
				p_threat->CoolProcess();
				p_threat->ChangeDir();
				AmoObject* p_amo = new AmoObject;
				p_threat->InitAmo(p_amo);
				p_threat->HandleMove(ROOM_X + ROOM_WIDTH - THREAT_WIDTH, SCREEN_HEIGHT);
				p_threat->MakeAmo(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				// xu li va cham dan vs threat
				if(!p_threat->checkAmour(human_object))
				{
					r_mark ++;
					p_threat->set_is_alive(0);
					p_threat->SetRect(SCREEN_WIDTH, SCREEN_HEIGHT);
				}
			}
		}
		 human_object.ShowStatic(g_screen);
		
		//test in game loop
		 
		//
	}
};
void prepare()
{
	//////menuBK
	menu.menuBk.SetWidthHeight(1056, 672);
	menu.menuBk.SetRect(72, 64);
	menu.menuBk.LoadImg("menu.png");



	///////menuITEM
	menu.menuArrow.LoadImg("arrow.png");
	menu.menuString[0] = "START";
	menu.menuString[1] = "NEW  GAME";
	menu.menuString[2] = "HOW  TO  PLAY";
	menu.menu_font = TTF_OpenFont("Mojang-Bold.ttf", 30);
	for (int i = 0; i < menu.numItems; i ++) 
	{
		int x = 375;
		int y = 350 + i * 110;
		menu.menuItems[i].SetWidthHeight(450, 97);
		menu.menuItems[i].SetRect(x, y);
		menu.menuItems[i].LoadImg("panel.png");
		
		menu.menuText[i].SetText(menu.menuString[i]);
		menu.menuText[i].SetColor(TextObject::BLACK_TEXT);
		menu.menuText[i].SetRect(x + 50, y + 35);
	}




	/////menuGUIDE
	menu.guideBk.SetRect(246, 46);
	menu.guideBk.LoadImg("guideBK.png");
	
	
}

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
	prepare();
	int x_mouse, y_mouse;
	
	bool is_quit = false;
	bool p_still_live = 1;
	long long numLoop = 0;
	round R1(0);
	R1.preapre();
	while (!is_quit) 
	{
		fps_timer.start();




		/////// SHOW MENU
		if(g_stat == START){
			menu.menuBk.Show(g_screen);
			SDL_Rect tmp = menu.menuArrow.GetRect();
			if(tmp.x != 0) menu.menuArrow.Show(g_screen);
			for (int i = 0; i < menu.numItems; i ++){
				menu.menuItems[i].Show(g_screen);
				menu.menuText[i].CreateGameText(menu.menu_font, g_screen);
			}
			while (SDL_PollEvent(&g_even)) 
			{
				switch (g_even.type)
				{
				case SDL_QUIT:
					return 1;
				case SDL_MOUSEMOTION:
					x_mouse = g_even.motion.x;
					y_mouse = g_even.motion.y;
					for (int i = 0; i < menu.numItems; i ++)
						if(SDLCommonFunc::CheckPointInRect(x_mouse, y_mouse, menu.menuItems[i].GetRect())){
							menu.menuText[i].SetColor(TextObject::RED_TEXT);
							menu.menuArrow.SetRect(275, 344 + i * 110);
						}
						else menu.menuText[i].SetColor(TextObject::BLACK_TEXT);
					break;
				case SDL_MOUSEBUTTONDOWN:
					x_mouse = g_even.motion.x;
					y_mouse = g_even.motion.y;
					for (int i = 0; i < menu.numItems; i ++)
						if(SDLCommonFunc::CheckPointInRect(x_mouse, y_mouse, menu.menuItems[i].GetRect()))
						{
							if(i == 0) g_stat = PLAYING;
							else if(i == 1) g_stat = PLAYING;
							else if(i == 2) g_stat = GUIDE;
						}
					break;
				default:
					break;
				}
			}
			if (SDL_Flip(g_screen) == -1)
				return 0;
		}

		//////guide page
		else if(g_stat == GUIDE)
		{
			menu.menuBk.Show(g_screen);
			menu.guideBk.Show(g_screen);
			

			while (SDL_PollEvent(&g_even)) 
			{
				switch (g_even.type)
				{
				case SDL_QUIT:
					return 1;	
				case SDL_KEYDOWN:
					{
					switch(g_even.key.keysym.sym)
					case SDLK_ESCAPE:
						g_stat = START;
						break;



					break;
					}
				default:
					break;
				}
			}

			
			




			if (SDL_Flip(g_screen) == -1)
				return 0;

		}




		///////playing game
		else if (g_stat == PLAYING){
			while (SDL_PollEvent(&g_even)) 
			{	
				if (g_even.type == SDL_QUIT)
				{
					is_quit = true;
					break;
				}
				human_object.HandleInputAction(g_even);
			}
			R1.gameLoop(is_quit, p_still_live, numLoop);
			// Update screen
			if (SDL_Flip(g_screen) == -1)
				return 0;
		}
		



		



		//////FPS handle
		int real_imp_time = fps_timer.get_tick();
		int time_one_frame = 1000/FRAME_PER_SECOND;
		if(real_imp_time < time_one_frame)
		{
 			int delay_time = time_one_frame - real_imp_time;
			SDL_Delay(delay_time);
		}
		numLoop ++;
		
	}
	//clean up variables in program
	SDLCommonFunc::CleanUp();
	SDL_Quit();
	return 1;
}