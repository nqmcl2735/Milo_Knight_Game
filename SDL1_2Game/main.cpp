#include "Common_Function.h"
#include "MainObject.h"
#include "ThreatObject.h"
#include "AmoObject.h"
#include <time.h>
#include <vector>
#include "ImpTimer.h" 
#include "TextObject.h"
#include "UserInterface.h"
#include "TileObject.h"
#include "fstream"
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

	BaseObject GuideBK;
} menu;
struct PauseMenu 
{
	TTF_Font * pauseFont ;
	TTF_Font * bodyFont ; 
	static const int numItems = 3;
	static const int num_f = 10 ; 
	BaseObject pauseBK ; 
	BaseObject pauseItem[numItems]; 
	BaseObject Skill_Items [numItems] ; 
	BaseObject avatar_of_player ; 
	std::string pauseString[numItems];
	std::string Display_Skill_str ;
	TextObject User_Data [2] ; 
	TextObject pauseText [numItems]; 
	TextObject Display_Skill ; 
	TextObject Menu_Title [2]; 
	bool is_show ;  
}pauseMenu;
struct pop_up
{
	TTF_Font * pop_font;
	static const int numItems = 2;
	BaseObject * Bk;
	TextObject * txt[numItems];
	SDL_Rect *P[numItems];
	pop_up()
	{
		pop_font = TTF_OpenFont("Mojang-Bold.ttf", 35);
		Bk = new BaseObject;
		Bk->LoadImg("pop_up - Copy.png");
		Bk->SetRect(360, 240);
		for (int i = 0; i < numItems; i ++){

			txt[i] = new TextObject;
			if(i) txt[i]->SetText("NO");
			else txt[i]->SetText("YES");
			txt[i]->SetRect(460 + i * 200, 430);
			txt[i]->SetColor(TextObject::BLACK_TEXT);
			
			P[i] = new SDL_Rect;
			P[i]->w = 148;
			P[i]->h = 84;
		}
		P[0]->x = 431;
		P[0]->y = 427;

		P[1]->x = 258 + 360;
		P[1]->y = 168 + 240;

	}
	~pop_up()
	{
		if(Bk != nullptr) delete Bk;
		for (int i = 0; i < numItems; i ++) {
			if(txt[i] != nullptr) delete txt[i];
			if(P[i] != nullptr) delete P[i];
		}
	}
	void Print()
	{
		Bk->Show(g_screen);
		for (int i = 0; i < numItems; i ++)
			txt[i]->CreateGameText(pop_font, g_screen);
	}
} *pop = nullptr;
enum gameStatus 
{
	START,
	PLAYING,
	PAUSE,
	GUIDE,
	POPUP,
	WINNER
};
int g_stat = START;



struct round 
{
	enum round_status
	{
		BEFORE,
		RUNNING,
		DONEROUND
	};
	int num_round;

	int r_mark;
	int r_stat;

	int NUM_THREAT;
	int NUM_TILE;
	
	// room objects
	BaseObject *room_object;
	BaseObject *border_object;
	BaseObject *Lgate_object;
	BaseObject *Rgate_object;
	BaseObject *road_object;
	round(const int & n_round)
	{
		num_round = n_round;
		r_mark = 0;
		r_stat = BEFORE;
	}
	~round()
	{
		delete room_object;
		delete border_object;
		delete Lgate_object;
		delete Rgate_object;
		delete road_object;
	}
	
	std::vector<ThreatObject*> p_threats;
	std::vector<TileObject*> p_tiles;
	//test outside game loop
	void Move(BaseObject * gate, int dir)
	{
		gate->SetRect(gate->GetRect().x, gate->GetRect().y + 5 * dir);
	}
	void before_frame()
	{
		if(Lgate_object->GetRect().y >= 165) Move(Lgate_object, -1);
	}
	void running_frame()
	{if(Lgate_object->GetRect().y < 335) Move(Lgate_object, 1);
	}
	void doneround_frame()
	{
		SDL_Rect tmp = Rgate_object->GetRect();
		if(tmp.y >= 165) Move(Rgate_object, -1);
	}
	void include_threat()
	{
		std::ifstream fi (("LEVEL1/ROUND" + std::to_string(num_round) + ".txt").c_str());
		fi>> NUM_THREAT;
		for (int t = 0; t < NUM_THREAT; t ++)
		{
			int type; fi >> type;
			ThreatObject *p_threat = new ThreatObject(type);
			p_threats.push_back(p_threat);
		}
		fi>>NUM_TILE;
		for (int t = 0; t < NUM_TILE; t ++)
		{
			int type, x, y; fi >> type >> x >> y;
			TileObject *p_tile = new TileObject(type, x, y);
			p_tiles.push_back(p_tile);
		}

	}
	void cleanup_threat()
	{
		p_threats.clear();
		p_tiles.clear();
		
	}
	
	//
	void preapre()
	{
		
		room_object = new BaseObject;
		border_object = new BaseObject;
		Lgate_object = new BaseObject;
		Rgate_object = new BaseObject;
		road_object = new BaseObject;

		room_object->SetRect(150, 25);
		room_object->LoadImg("room0.png");
		border_object->SetRect(150, 25);
		border_object->LoadImg("border.png");
		Lgate_object->SetRect(150, 335);
		Lgate_object->SetWidthHeight(25, 145);
		Lgate_object->LoadImg("Gate.png");
		Rgate_object->SetRect(1025, 335);
		Rgate_object->SetWidthHeight(25, 145);
		Rgate_object->LoadImg("Gate.png");
		road_object->SetRect(0, 310);
		road_object->LoadImg("road.png");
		
		human_object.prepare(g_stat);
		
		
	}

	bool gameLoop(bool &is_quit, bool &p_still_live, long long numLoop)
	{
		
		//update round_status
		if(r_stat == BEFORE)
		{
			SDL_Rect tmp = human_object.GetRect();
			if(tmp.x >= 180){ 
				r_stat = RUNNING;
				include_threat();
				return 0;
			}
		}
		else if(r_stat == RUNNING)
		{
			if(r_mark == NUM_THREAT)
			{
				cleanup_threat();
				r_stat = DONEROUND;
				return 0;
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
		road_object->Show(g_screen);
		room_object->Show(g_screen);
		border_object->Show(g_screen);
		Lgate_object->Show(g_screen);
		Rgate_object->Show(g_screen);
		
		//Implement main object
		if(r_stat == RUNNING)
			for (int i = 0; i < NUM_TILE; i ++)
			{
				TileObject *p_tile = p_tiles.at(i);
				p_tile->Show(g_screen);
			}
		human_object.Show(g_screen);
		if(human_object.HandleMove(Lgate_object, Rgate_object, p_tiles)) return 1;
		human_object.MakeAmo(g_screen);
		human_object.coolProcess();
		

		//xu li va cham dan vs main
		if(r_stat != RUNNING) return 0;
		
		bool is_slow = 0;
		bool is_poison = 0;
		for (int i = 0; i < p_threats.size(); i ++)
		{
			ThreatObject *p_threat = p_threats.at(i);
			if(p_threat->get_is_alive()){
				//gun
				if(p_threat->get_type() == ThreatObject::GUN_THREAT){
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
	
									break;
								}
								if(am < amo_list.size()) amo_list.erase(amo_list.begin() + am);
								p_threat->SetAmoList(amo_list);
							}
						}
						if(p_still_live == 0) return 0;
					}
					if(p_still_live == 0) return 0;
				}
				//
				else if(p_threat->get_type() == ThreatObject::TANK_THREAT)
				{
					if(SDLCommonFunc::CheckCollision(human_object.GetRect(), p_threat->GetRect()))
					{
						is_poison = 1;
						is_slow = 1;
					}
				}
			}
		}
		if(is_slow)  human_object.set_slow(1);
		if(is_poison)
			human_object.set_poison(1);
		if(human_object.get_poison()){
			if(human_object.get_shield() > 0)
			{
				int p_shield = human_object.get_shield();
				human_object.set_shield(max(p_shield - 1, 0));
			}
			else human_object.change_health((-1) * (100.0/100.0 * 1));
			if(human_object.get_health() <= 0) 
				p_still_live = 0;
		}
		if(p_still_live == 0) return 0;
		//

		// Implement threat object
		for (int i = 0; i < p_threats.size(); i ++)
		{
			//std::vector<ThreatObject*> threat_list = p_threats;
			ThreatObject *p_threat = p_threats.at(i);
			if(p_threat->get_is_alive()){
				p_threat->Show(g_screen);
				if(p_threat->get_type() == ThreatObject::GUN_THREAT){
					p_threat->CoolProcess();
					p_threat->ChangeDir(human_object);
					AmoObject* p_amo = new AmoObject;
					if(!p_threat->InitAmo(p_amo))
					{
						delete p_amo;
					}
					p_threat->HandleMove(p_tiles);
					p_threat->MakeAmo(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				}
				else if (p_threat->get_type() == ThreatObject::TANK_THREAT)
				{
					p_threat->frame_load();
					p_threat->CoolProcess();
					p_threat->ChangeDir(human_object);
					p_threat->HandleMove(p_tiles);
				}
				// xu li va cham dan vs threat
				if(!p_threat->checkAmour(human_object))
				{
					r_mark ++;
					p_threat->set_is_alive(0);
					p_threat->SetRect(SCREEN_WIDTH, SCREEN_HEIGHT);
				}
			}
		}
		 human_object.ShowStatic(g_screen, 1);
		return 0;
		//test in game loop
		 
		//
	}
};
void menu_prepare()
{
	menu.menuBk.SetWidthHeight(1056, 672);
	menu.menuBk.SetRect(72, 64);
	menu.menuBk.LoadImg("menu.png");
	menu.menuArrow.LoadImg("arrow.png");
	
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
	menu.GuideBK.LoadImg("guideBK.png");
	menu.GuideBK.SetRect(246, 46);


}
void prepare_pause () 
{
	pauseMenu.pauseBK.SetWidthHeight(970,750) ; 
	pauseMenu.pauseBK.SetRect(-185,-335) ; 
	pauseMenu.is_show = false ; 
	pauseMenu.avatar_of_player.LoadImg("Avatar.jpg") ; 
	pauseMenu.avatar_of_player.SetRect(220,180) ; 
	pauseMenu.bodyFont = TTF_OpenFont("Mojang-Bold.ttf",20) ; 
	pauseMenu.pauseFont = TTF_OpenFont("Mojang-Bold.ttf", 35);
	pauseMenu.Display_Skill.SetText("Skill :") ; 
	pauseMenu.Display_Skill.SetColor(TextObject::BLACK_TEXT) ; 
	pauseMenu.Display_Skill.SetRect (660,180) ;
	pauseMenu.Menu_Title[0].SetText("________PROFILE________") ; 
	pauseMenu.Menu_Title[1].SetText("________SETTING________") ;
	pauseMenu.User_Data[0].SetText("Mi-Lo Knight") ; 
	pauseMenu.User_Data[1].SetText("Cre : deptrai2k5 (long nguyen) xautrai2k5 (minhnguyen)" ) ; 
	for (int i = 0 ;i < 2 ; i ++ ) 
	{
	pauseMenu.Menu_Title[i].SetRect(230,120 + 245 * i) ; 
	pauseMenu.Menu_Title[i].SetColor(TextObject :: WHITE_TEXT) ;
	pauseMenu.User_Data[i].SetRect(205 - 10 * i , 330 + 300 * i) ;
	pauseMenu.User_Data[i].SetColor(TextObject :: BLACK_TEXT) ; 
	}
	pauseMenu.Skill_Items[0].LoadImg("skillQ.png") ; 
	pauseMenu.Skill_Items[1].LoadImg("skillE.png") ; 
	pauseMenu.Skill_Items[2].LoadImg("skillUlt.png") ; 
	pauseMenu.pauseString[0] = "MENU" ; 
	pauseMenu.pauseString[1] = "RESUME" ; 
	pauseMenu.pauseString[2] = "H 2 P" ; 
	for (int i = 0 ; i < pauseMenu.numItems ; i ++ ) 
	{
		
		pauseMenu.Skill_Items[i].SetRect (660 + 115 * i , 210) ; 
		pauseMenu.pauseItem[i].LoadImg("HUD_image.png") ; 
		pauseMenu.pauseItem[i].SetWidthHeight(235,123) ; 
		pauseMenu.pauseItem[i].SetRect(220 + 260 * i , 450) ; 
		pauseMenu.pauseText[i].SetColor(TextObject :: BLACK_TEXT) ; 
		if (i % 2 == 0 ) pauseMenu.pauseText[i].SetRect(280 + 260 * i , 493 ) ; 
		else pauseMenu.pauseText[i].SetRect(245 + 260 * i , 493 ) ; 
		pauseMenu.pauseText[i].SetText(pauseMenu.pauseString[i]) ; 
	}
}
int max_round = 0;
int winner_count = 0;
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
	menu_prepare();
	prepare_pause ();
	int x_mouse, y_mouse;
	
	bool is_quit = false;
	bool p_still_live = 1;
	long long numLoop = 0;
	max_round = 0;
	round *now_round = nullptr;
	bool is_first_move = 0;
	int now_health = 600;
	int now_AD = 60;
	int now_AP = 30;
	while (!is_quit) 
	{
		fps_timer.start();




		/////// SHOW MENU
		if(g_stat == START){
			std::ifstream fi("checkpoint.txt");
			int has_cp; fi >> has_cp;
			if(has_cp) 
				menu.menuString[0] = "CONTINUE";
			else menu.menuString[0] = "START";
			menu.menuText[0].SetText(menu.menuString[0]);
			SDLCommonFunc :: ApplySurface (g_bkground , g_screen ,0,0); 
			menu.menuBk.Show(g_screen);
			if(menu.menuArrow.GetRect().x != 0) menu.menuArrow.Show(g_screen);
			for (int i = 0; i < menu.numItems; i ++){
				menu.menuItems[i].Show(g_screen);
				menu.menuText[i].CreateGameText(menu.menu_font, g_screen);
			}
			while (SDL_PollEvent(&g_even)) 
			{
				switch (g_even.type)
				{
				case SDL_QUIT:
					SDLCommonFunc ::CleanUp () ; 
					SDL_Quit() ; 
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
							if(i == 0){ 
								if(has_cp == 0) {
									winner_count = 0;
									g_stat = PLAYING;
									human_object.reset(0);
									now_health = 600;
									now_AD = 60;
									now_AP = 30;
									max_round = 0;
									if(now_round != nullptr) delete now_round;
									now_round = new round(0);
									now_round->preapre();
								}
								else if (has_cp == 1)
								{
									winner_count = 0;
									g_stat = PLAYING;
									fi >> max_round;
									int health, AD, AP;
									fi >> health >> AD >> AP;
									now_health = health;
									now_AD = AD;
									now_AP = AP;
									human_object.load_file(health, AD, AP);
									if(now_round != nullptr) delete now_round;
									now_round = new round(max_round);
									now_round->preapre();
									fi.close();
								}
							}
							else if(i == 1) {
								winner_count = 0;
								g_stat = PLAYING;
								human_object.reset(0);
								now_health = 600;
								now_AD = 60;
								now_AP = 30;
								max_round = 0;
								if(now_round != nullptr) delete now_round;
								now_round = new round(0);
								now_round->preapre();
							}
							else if (i == 2) g_stat = GUIDE;
						}
					break;
				default:
					break;
				}
			}
			if (SDL_Flip(g_screen) == -1)
				return 0;
		}
		else if(g_stat == POPUP)
		{
			SDLCommonFunc :: ApplySurface (g_bkground , g_screen ,0,0);
			pop->Print();
			if (SDL_Flip(g_screen) == -1)
				return 0;
			while (SDL_PollEvent(&g_even)) 
			{
			switch (g_even.type)
				{
				case SDL_QUIT:
					SDLCommonFunc ::CleanUp () ; 
					SDL_Quit() ; 
					return 1;
				case SDL_MOUSEMOTION:
					x_mouse = g_even.motion.x;
					y_mouse = g_even.motion.y;
					//if else 
					for (int i = 0; i < pop->numItems; i ++)
					{
						if(SDLCommonFunc::CheckPointInRect(x_mouse, y_mouse, *pop->P[i])){
							pop->txt[i]->SetColor(TextObject::RED_TEXT);
						}
						else pop->txt[i]->SetColor(TextObject::BLACK_TEXT);
					}
					
					break;
				case SDL_MOUSEBUTTONDOWN:
					x_mouse = g_even.motion.x;
					y_mouse = g_even.motion.y;
					//if else
					for (int i = 0; i < pop->numItems; i ++)
					{
						if(SDLCommonFunc::CheckPointInRect(x_mouse, y_mouse, *pop->P[i])){
							if(i == 0)
							{
								std::ofstream fo("checkpoint.txt");
								fo << 1 << "\n";
								fo << max_round << "\n";
								fo << now_health << " ";
								fo << now_AD << " ";
								fo << now_AP << " ";
								fo.close();
							}
						}
					}
					delete pop;
					g_stat = START;
					break;
				default:
					break;
				}
			}
		}

		//// Pause Menu 
		else if (g_stat == PAUSE) 
		{
			if (!pauseMenu.is_show)
			{
				human_object.prepare(g_stat) ;
				for (int i = 0 ; i <= pauseMenu.num_f ; i ++) 
				{
					SDLCommonFunc :: ApplySurface (g_bkground , g_screen , 0 ,0 );
					std :: string temp = "pngs/Appear/"+ std ::to_string (i) + ".png" ; 
					pauseMenu.pauseBK.LoadImg(temp.c_str()) ; 
					pauseMenu.pauseBK.Show(g_screen) ;  
					if (SDL_Flip(g_screen) == -1)
					return 0;
					SDL_Delay(20) ;
				}
			}
			SDLCommonFunc :: ApplySurface (g_bkground , g_screen, 0 ,0  ) ;
			pauseMenu.is_show = true ;
			pauseMenu.pauseBK.Show(g_screen) ;
			pauseMenu.avatar_of_player.Show(g_screen) ; 
			human_object.ShowStatic(g_screen, 0) ; 
			pauseMenu.Display_Skill.CreateGameText(pauseMenu.bodyFont,g_screen) ;
			for (int i = 0 ;i < pauseMenu.numItems ; i ++ ) 
			{
				if (i < 2) 
					{
						pauseMenu.Menu_Title[i].CreateGameText(pauseMenu.pauseFont,g_screen) ;
						pauseMenu.User_Data[i].CreateGameText(pauseMenu.bodyFont,g_screen);
					}
				pauseMenu.Skill_Items[i].Show(g_screen) ;
				pauseMenu.pauseItem[i].Show(g_screen) ; 
				pauseMenu.pauseText[i].CreateGameText(pauseMenu.pauseFont,g_screen) ; 
			}
			while (SDL_PollEvent (&g_even) && g_stat == PAUSE) 
			{
				int x_mouse = g_even.motion.x , y_mouse = g_even.motion.y ;
				if (g_even.type == SDL_QUIT )
				{
					SDLCommonFunc :: CleanUp () ; 
					SDL_Quit() ; 
					return 0 ; 
				}
				else if (g_even.key.keysym.sym == SDLK_BACKSPACE ) 
				{
					pauseMenu.is_show = false ; 
					g_stat = PLAYING ; 
					human_object.prepare(g_stat) ; 
					for (int i = 0 ; i <= pauseMenu.num_f ; i ++ ) 
					{
					SDLCommonFunc :: ApplySurface (g_bkground , g_screen , 0 , 0 ) ; 
					std :: string temp = "pngs/Disappear/" + std :: to_string (i) + ".png" ;  
					pauseMenu.pauseBK.LoadImg(temp.c_str()) ; 
					pauseMenu.pauseBK.Show(g_screen) ; 
					if (SDL_Flip(g_screen) == -1)
					return 0;
					SDL_Delay(20) ; 
					}
					break ; 
				}
				switch (g_even.type) 
				{
				case SDL_MOUSEMOTION :  
					for (int i = 0 ; i < pauseMenu.numItems ; i ++ ) 
					{
						if (SDLCommonFunc :: CheckPointInRect (x_mouse , y_mouse , pauseMenu.pauseItem[i].GetRect())) 
							pauseMenu.pauseText[i].SetColor(TextObject :: RED_TEXT ) ; 
						else pauseMenu.pauseText[i].SetColor(TextObject :: BLACK_TEXT) ; 
						//	
						pauseMenu.pauseText[i].CreateGameText(pauseMenu.pauseFont,g_screen) ; 
					if (SDL_Flip(g_screen) == -1)
					return 0;
					}
					break ; 
				case SDL_MOUSEBUTTONDOWN: 
					for (int i = 0 ; i < pauseMenu.numItems ; i ++ ) 
					{
						if (SDLCommonFunc :: CheckPointInRect (x_mouse , y_mouse , pauseMenu.pauseItem[i].GetRect()))
						{
							switch (i) 
							{
							case 0 :
								pauseMenu.is_show = false ;  
								for (int i = 0 ; i <= pauseMenu.num_f ; i ++ ) 
								{
									SDLCommonFunc :: ApplySurface (g_bkground , g_screen , 0 , 0 ) ; 
									std :: string temp = "pngs/Disappear/" + std :: to_string (i) + ".png" ;  
									pauseMenu.pauseBK.LoadImg(temp.c_str()) ; 
									pauseMenu.pauseBK.Show(g_screen) ; 
									if (SDL_Flip(g_screen) == -1)
										return 0;
									SDL_Delay(20) ; 
								}
								// them option luu file
								
								pop = new pop_up;
								g_stat = POPUP;
								human_object.prepare(g_stat);
								break ; 
							case 1 : 					
								pauseMenu.is_show = false ; 
								g_stat = PLAYING ; 
								human_object.prepare(g_stat) ; 
								for (int i = 0 ; i <= pauseMenu.num_f ; i ++ ) 
								{
									SDLCommonFunc :: ApplySurface (g_bkground , g_screen , 0 , 0 ) ; 
									std :: string temp = "pngs/Disappear/" + std :: to_string (i) + ".png" ;  
									pauseMenu.pauseBK.LoadImg(temp.c_str()) ; 
									pauseMenu.pauseBK.Show(g_screen) ; 
									if (SDL_Flip(g_screen) == -1)
										return 0;
									SDL_Delay(20) ; 
								}
								break ;
							case 2 : 
								g_stat = GUIDE;
								break;
							} 
					}
					}
				}
			}
				if (SDL_Flip(g_screen) == -1)
				return 0;
		}

		//winner winner chicken dinner
		
			




		///guide
		else if (g_stat == GUIDE)
		{
			SDLCommonFunc::ApplySurface(g_bkground, g_screen, 0, 0);
			while (SDL_PollEvent(&g_even)) 
			{	
				if (g_even.type == SDL_QUIT)
				{
					is_quit = true;
					break;
				}
				if (g_even.key.keysym.sym == SDLK_ESCAPE)
				{
					human_object.prepare(g_stat);
					g_stat = START; 
					break; 
				}
			}
			menu.GuideBK.Show(g_screen);
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
				if (g_even.key.keysym.sym == SDLK_ESCAPE)
				{
						g_stat = PAUSE ; 
						human_object.stop();
						break; 
				}
				if(is_first_move == 0) human_object.HandleInputAction(g_even);
				else is_first_move = 0;
				
			}
			if (g_stat == PAUSE) continue ;
			if(now_round->gameLoop(is_quit, p_still_live, numLoop))
			{
				if(max_round == 5)
				{
					SDLCommonFunc::ApplySurface(g_bkground, g_screen, 0, 0);
					BaseObject* Bk = new BaseObject;
					Bk->LoadImg("winner.png");
					Bk->SetRect(72, 64);
					Bk->Show(g_screen);
					delete Bk;
					is_first_move = 1;
					winner_count ++;
					if(winner_count == 80) {
						g_stat = START;
						std::ofstream fo("checkpoint.txt");
						fo << 0;
						fo.close();
					}
				}
				else {
					winner_count = 0;
					now_health = human_object.get_health();
					now_AD = human_object.get_AD_pow();
					now_AP = human_object.get_AP_pow();

					human_object.reset(1);
					human_object.stop();
					max_round ++;
					delete now_round;
					now_round = new round(max_round);
					now_round->preapre();
					is_first_move = 1;
				}
			}
			// Update screen
			
			if(p_still_live == 0){
				/*
				them option luu file
				*/
				pop = new pop_up;
				g_stat = POPUP;
				human_object.prepare(g_stat); 
				p_still_live = 1;
			}
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